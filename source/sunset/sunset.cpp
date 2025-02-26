#include "sunset.hpp"

std::vector<sunset::utils::JitMemory> sunset::detail::JIT_MEMORY{};
std::mutex sunset::detail::JIT_MEMORY_LOCK;