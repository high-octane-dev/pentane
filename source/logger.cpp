#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <Windows.h>

#include "logger.hpp"
#include "localization.hpp"
#include "config.hpp"
#include "util/mutex.hpp"

class Logger {
private:
    FILE* console_file;
    std::ofstream log_file;
public:
    Logger() : console_file(nullptr) {}

    ~Logger() {
        if (console_file != nullptr) {
            fclose(console_file);
            console_file = nullptr;
        }
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    bool init_file_logging(const std::filesystem::path& file_path) {
        log_file.open(file_path);
        if (!log_file.is_open()) {
            return false;
        }
        return true;
    }

    bool init_console_logging() {
        AllocConsole();

        freopen_s(&console_file, "CONOUT$", "w", stdout);
        if (console_file == nullptr) {
            return false;
        }

        SetConsoleOutputCP(CP_UTF8);

        std::size_t required_length = MultiByteToWideChar(CP_UTF8, 0, PENTANE_CONSOLE[config::language()].data(), PENTANE_CONSOLE[config::language()].length(), nullptr, 0);
        std::wstring converted{};
        converted.resize(required_length);
        MultiByteToWideChar(CP_UTF8, 0, PENTANE_CONSOLE[config::language()].data(), PENTANE_CONSOLE[config::language()].length(), converted.data(), required_length);
        
        SetConsoleTitleW(converted.data());
        return true;
    }

    void log(const std::string_view& str) {
        if (console_file != nullptr) {
            std::cout << str << std::endl;
        }
        if (log_file.is_open()) {
            log_file << str << std::endl;
        }
    }
};

util::Mutex<std::unique_ptr<Logger>> LOGGER{};

bool logger::init(const std::filesystem::path& file_path, bool create_console, bool log_to_file) {
    auto guard = LOGGER.lock_mut();
    *guard = std::make_unique<Logger>();
    bool created_console = false;
    bool created_file = false;
    if (create_console) {
        created_console = (*guard)->init_console_logging();
    }
    if (log_to_file) {
        created_file = (*guard)->init_file_logging(file_path);
    }
    return created_console && create_console || created_file && log_to_file;
}

void logger::log(const std::string_view& str) {
    auto guard = LOGGER.lock_mut();
    (*guard)->log(str);
}
