#include <filesystem>
#include <ranges>
#include <mutex>
#include <unordered_set>
#include <Windows.h>
#include "api/api_macros.hpp"
#include "config.hpp"
#include "plugin.hpp"
#include "plugin_loader.hpp"
#include "localization.hpp"
#include "logger.hpp"
#include "util/sorted_hashmap.hpp"
#include "version.hpp"

struct ProcessedPlugin {
    std::string name;
    std::string author;
    PentaneUUID uuid;
    SemVer version;
    std::vector<PentaneUUID> dependencies_uuids;
    PentaneMainFunc main_function;
};

std::unordered_set<PentaneUUID, PentaneUUIDHasher> LOADED_PLUGIN_UUIDS{};
std::mutex LOADED_PLUGIN_UUIDS_LOCK{};

auto plugin_loader::is_loaded(PentaneUUID* uuid) -> bool {
    std::scoped_lock<std::mutex> lock(LOADED_PLUGIN_UUIDS_LOCK);
    if (uuid != nullptr) {
        return LOADED_PLUGIN_UUIDS.contains(*uuid);
    }
    return false;
}

auto mount_plugin_and_get_main(const std::wstring& path_string) -> void(__stdcall*)() {
    HMODULE h_module = LoadLibraryExW(path_string.data(), nullptr, 0);
    if (h_module != nullptr) {
        PentaneMainFunc plugin_pentane_main = reinterpret_cast<PentaneMainFunc>(GetProcAddress(h_module, API_PLUGIN_ENTRY_POINT));
        if (plugin_pentane_main == nullptr) {
            FreeLibrary(h_module);
            return nullptr;
        }
        return plugin_pentane_main;
    }
    return nullptr;
}

// Rejects plugins that:
//  1. Do not export their `Pentane_PluginInformation`.
//  2. Require a newer version of Pentane.
//  3. Do not export a `Pentane_Main` function.
//  4. Are not the newest version amongst all their duplicates (plugins with the same UUID).
auto process_candidate(util::SortedHashMap<PentaneUUID, ProcessedPlugin, PentaneUUIDHasher>& processed_plugins, const std::wstring& path_string, const std::string& base_file_name) -> void {
    // Temporarily load the plugin so we can grab its `PluginInformation` and verify the version.
    HMODULE h_module = LoadLibraryExW(path_string.data(), nullptr, 0);
    
    if (h_module == nullptr) {
        LOG_LOCALIZED_STRING(MODULE_REJECTED, base_file_name);
        return;
    }

    PluginInformation* plugin_info_ptr = reinterpret_cast<PluginInformation*>(GetProcAddress(h_module, API_PLUGIN_INFORMATION));
    if (plugin_info_ptr == nullptr) {
        LOG_LOCALIZED_STRING(MODULE_REJECTED_MISSING_INFO_STRUCT_EXPORT, base_file_name);
        return;
    }

    std::vector<PentaneUUID> plugin_dependencies{};
    PentaneUUID* plugin_dependencies_ptr = nullptr;
    
    std::size_t* plugin_dependency_count_ptr = reinterpret_cast<std::size_t*>(GetProcAddress(h_module, API_PLUGIN_DEPENDENCY_COUNT));
    if (plugin_dependency_count_ptr == nullptr) {
        LOG_LOCALIZED_STRING(MODULE_REJECTED_MISSING_DEPENDENCY_COUNT_EXPORT, base_file_name);
        return;
    }
    
    if (*plugin_dependency_count_ptr != 0) {
        plugin_dependencies_ptr = reinterpret_cast<PentaneUUID*>(GetProcAddress(h_module, API_PLUGIN_DEPENDENCIES));
        if (plugin_dependencies_ptr == nullptr) {
            LOG_LOCALIZED_STRING(MODULE_REJECTED_MISSING_DEPENDENCIES_EXPORT, base_file_name);
            return;
        }
        // We only want to copy the exported dependencies into our vector if the size isn't zero, to avoid UB.
        plugin_dependencies.resize(*plugin_dependency_count_ptr);
        std::memcpy(plugin_dependencies.data(), plugin_dependencies_ptr, *plugin_dependency_count_ptr * sizeof(PentaneUUID));
    }

    PluginInformation info = *plugin_info_ptr;
    FreeLibrary(h_module);

    // If the current version does not exceed or match the plugin's minimum required version of Pentane, do not mount it.
    if (PENTANE_FRAMEWORK_VERSION < info.minimum_pentane_version) {
        LOG_LOCALIZED_STRING(MODULE_REJECTED_PENTANE_OOD, info.name, info.version.major, info.version.minor, info.version.patch);
        return;
    }

    if (auto main_function = mount_plugin_and_get_main(path_string); main_function != nullptr) {
        // If we have already added a plugin with the same UUID, we need to either:
        // 1. Remove the previous entry if the current one is newer.
        // 2. Reject the current one if it is older or the same version.
        if (auto index = processed_plugins.get_index(info.uuid); index.has_value()) {
            const auto& plugin = processed_plugins[index.value()].value();
            if (plugin.version < info.version) {
                processed_plugins.erase(index.value());
            }
            else {
                LOG_LOCALIZED_STRING(MODULE_REJECTED_OLDER_DUPLICATE);
                return;
            }
        }

        ProcessedPlugin processed_data{
            .name = info.name,
            .author = info.author,
            .uuid = info.uuid,
            .version = info.version,
            .dependencies_uuids = plugin_dependencies,
            .main_function = main_function,
        };
        processed_plugins.insert(info.uuid, processed_data);
    }
    else {
        LOG_LOCALIZED_STRING(MODULE_REJECTED_MISSING_MAIN, info.name, info.version.major, info.version.minor, info.version.patch);
    }
}

auto check_dependencies_recursive(const util::SortedHashMap<PentaneUUID, ProcessedPlugin, PentaneUUIDHasher>& processed_plugins, std::unordered_set<PentaneUUID, PentaneUUIDHasher>& encountered_uuids, PentaneUUID mod) -> bool {
    if (encountered_uuids.contains(mod)) {
        return true;
    }
    
    if (auto plugin = processed_plugins[mod]; plugin.has_value()) {
        for (const auto& dependency : plugin.value().dependencies_uuids) {
            if (!processed_plugins.contains(mod)) {
                return false;
            }
            if (!check_dependencies_recursive(processed_plugins, encountered_uuids, dependency)) {
                return false;
            }
        }
        encountered_uuids.insert(mod);
        return true;
    }
    return false;
}

// Recursively checks every plugin's dependency to ensure *EVERY SINGLE DEPENDENCY AS WELL AS ITS DEPENDENCIES* are satisfied, and filters out those that don't.
auto check_dependencies(const util::SortedHashMap<PentaneUUID, ProcessedPlugin, PentaneUUIDHasher>& processed_plugins) -> std::vector<ProcessedPlugin> {
    std::unordered_set<PentaneUUID, PentaneUUIDHasher> uuids_encountered{};
    uuids_encountered.reserve(processed_plugins.size());

    std::vector<ProcessedPlugin> filtered_plugins{};
    filtered_plugins.reserve(processed_plugins.size());

    for (std::size_t i = 0; i < processed_plugins.size(); i++) {
        if (check_dependencies_recursive(processed_plugins, uuids_encountered, processed_plugins[i].value().uuid)) {
            filtered_plugins.push_back(processed_plugins[i].value());
        }
        
        else {
            std::string plugin_name = processed_plugins[i].value().name;
            LOG_LOCALIZED_STRING(MODULE_REJECTED_MISSING_DEPENDENCY, plugin_name);
        }
    }
    return filtered_plugins;
}

// Processes every plugin candidate in the folder, and then filters out the plugins that don't have their dependencies set.
auto collect_and_validate_plugins(const std::filesystem::path& plugin_dir) -> std::vector<ProcessedPlugin> {
    util::SortedHashMap<PentaneUUID, ProcessedPlugin, PentaneUUIDHasher> processed_plugins{};
    if (std::filesystem::is_directory(plugin_dir)) {
        std::vector<std::string> enabled_plugins = config::plugins_enabled();
        // Iterate over the enabled plugins in *reverse* order, so that the lowest priority plugins' main functions are ran first.
        for (const auto& plugin_file_name : enabled_plugins | std::views::reverse) {
            std::filesystem::path full_path = plugin_dir / plugin_file_name;
            if (std::filesystem::is_regular_file(full_path)) {
                std::wstring full_path_string = full_path.native();
                process_candidate(processed_plugins, full_path_string, plugin_file_name);
            }
            else {
                LOG_LOCALIZED_STRING(MODULE_REJECTED_NOT_FOUND, plugin_file_name);
            }
        }
    }
    return check_dependencies(processed_plugins);
}

auto plugin_loader::load_all(const std::filesystem::path& plugin_dir) -> void {
    const auto& plugins_to_load = collect_and_validate_plugins(plugin_dir);

    // Here, we copy all the valid plugins' UUIDs into the LOADED_PLUGIN_UUIDS *before* we call any main functions, so the set is ready to be used by that time. 
    for (const auto& plugin : plugins_to_load) {
        LOADED_PLUGIN_UUIDS.insert(plugin.uuid);
    }

    // In this final pass, we reject all modules that do not have all their dependencies present.
    for (const auto& plugin : plugins_to_load) {
        std::string a = plugin.name;
        LOG_LOCALIZED_STRING(PLUGIN_LOAD_SUCCESS, plugin.name, plugin.version.major, plugin.version.minor, plugin.version.patch, plugin.author);
        plugin.main_function();
    }
}
