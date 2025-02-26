#include <cstdint>
#include <cstddef>
#include <windows.h>
#include "tvg_file_system.hpp"

#pragma pack(push, 1) // Ensure packing alignment

struct FileInfo {
    char path[100];
    std::uint32_t offset;
    std::uint32_t size;
};

struct LoadedPak {
    char path[100];
    FileInfo* files_in_pak;
    std::size_t file_count;
    std::uint32_t file_data_start_offset;
};

struct LoadedFile {
    void* pak_file_pointer;
    FileInfo* file_info;
    const LoadedPak* loaded_pak;
    std::uint32_t seek;
};

struct PakSystem {
    LoadedPak* loaded_paks;
    std::uint32_t pak_count;
    std::uint32_t unk;
    RTL_CRITICAL_SECTION lock;
};

#pragma pack(pop) // Restore packing alignment

static LoadedPak DUMMY_LOADED_PAK = LoadedPak{
    .path = {},
    .files_in_pak = 0,
    .file_count = 0,
    .file_data_start_offset = 0,
};