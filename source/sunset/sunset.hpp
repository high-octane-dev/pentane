#pragma once

#include <iostream>
#include <format>
#include <cstdint>
#include <vector>
#include <mutex>
#include <type_traits>
#include <Windows.h>
#include <detours/detours.h>

#include "relocate_code.hpp"

#define DefineReplacementHook(name) \
struct name : public sunset::detail::ReplacementHook<name>

#define DefineInlineHook(name) \
struct name : public sunset::detail::InlineHook<name>

namespace sunset {

    namespace utils {
        enum class Perm : DWORD {
            None = PAGE_NOACCESS,
            Read = PAGE_READONLY,
            ReadWrite = PAGE_READWRITE,
            WriteCopy = PAGE_WRITECOPY,
            Execute = PAGE_EXECUTE,
            ExecuteRead = PAGE_EXECUTE_READ,
            ExecuteReadWrite = PAGE_EXECUTE_READWRITE,
            ExecuteWriteCopy = PAGE_EXECUTE_WRITECOPY,
            Guard = PAGE_GUARD,
            NoCache = PAGE_NOCACHE,
            WriteCombine = PAGE_WRITECOMBINE,
        };
        // Sets the desired permission on the memory block.
        template <typename T, typename std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
        inline std::pair<Perm, bool> set_permission(T ptr, size_t size, Perm perm) {
            Perm old_perm = Perm::None;
            bool success = static_cast<bool>(VirtualProtect(reinterpret_cast<void*>(ptr), size, static_cast<DWORD>(perm), reinterpret_cast<DWORD*>(&old_perm)));
            return std::make_pair(old_perm, success);
        }

        class JitMemory {
        public:
            std::uint8_t* data;
            std::size_t len;
	private:
            inline void destroy_impl() {
                if (data != nullptr) {
                    VirtualFree(reinterpret_cast<void*>(data), 0, MEM_RELEASE);
                    data = nullptr;
                    len = 0;
                }
            }
        public:
            inline JitMemory(std::size_t size) {
                data = reinterpret_cast<std::uint8_t*>(VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
                len = size;
            }
            JitMemory() = delete;
            JitMemory(const JitMemory&) = delete;
            JitMemory& operator=(const JitMemory&) = delete;
            inline JitMemory(JitMemory&& other) noexcept : data(nullptr), len(0) {
                data = other.data;
                len = other.len;
                other.data = nullptr;
                other.len = 0;
            }
            inline JitMemory& operator=(JitMemory&& other) noexcept {
                if (this != &other) {
                    destroy_impl();
                    data = other.data;
                    len = other.len;
                    other.data = nullptr;
                    other.len = 0;
                }
                return *this;
            }
            // All these hoops to jump through just for psuedo-destructive moves...
            inline ~JitMemory() {
                destroy_impl();
            }
        };
    };

    template <typename T1, typename std::enable_if_t<std::is_pointer_v<T1>>* = nullptr, typename T2, typename std::enable_if_t<std::is_pointer_v<T2>>* = nullptr>
    inline void write_jmp(T1 src, T2 dst) {
        uintptr_t relativeAddress = (uintptr_t)((std::uint8_t*)dst - (uintptr_t)src) - 5;

        utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0xE9;
        *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;
    }

    template <typename T1, typename std::enable_if_t<std::is_pointer_v<T1>>* = nullptr, typename T2, typename std::enable_if_t<std::is_pointer_v<T2>>* = nullptr>
    inline void write_call(T1 src, T2 dst) {
        uintptr_t relativeAddress = (uintptr_t)((std::uint8_t*)dst - (uintptr_t)src) - 5;

        utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0xE8;
        *(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;
    }

    template <typename T1, typename std::enable_if_t<std::is_pointer_v<T1>>* = nullptr, typename T2, typename std::enable_if_t<std::is_integral_v<T2>>* = nullptr>
    inline void write_push(T1 src, T2 dst) {
        utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite);
        *(std::uint8_t*)src = 0x68;
        *(uintptr_t*)((uintptr_t)src + 1) = (uintptr_t)dst;
    }

    template <typename T, typename std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
    inline bool write_nop(T addr, std::size_t code_size) {
        const auto& [original_protection, success] = utils::set_permission(addr, code_size, utils::Perm::ExecuteReadWrite);
        if (success) {
            std::memset(reinterpret_cast<void*>(addr), 0x90, code_size);
            return true;
        }
        return false;
    }

    namespace legacy {
        template <typename T1, typename std::enable_if_t<std::is_pointer_v<T1>>* = nullptr, typename T2, typename std::enable_if_t<std::is_pointer_v<T2>>* = nullptr>
        inline void inline_replace(T1 src, T2 dst, std::size_t size) {
            write_nop(src, size);
            write_call(src, dst);
        }
        template <typename T1, typename std::enable_if_t<std::is_pointer_v<T1>>* = nullptr, typename T2, typename std::enable_if_t<std::is_pointer_v<T2>>* = nullptr>
        inline void inline_replace_jump(T1 src, T2 dst, std::size_t size) {
            write_nop(src, size);
            write_jmp(src, dst);
        }
    };

    union Register {
        void* pointer;
        std::uint32_t unsigned_integer;
        std::int32_t signed_integer;
        float floating_point;
    };

    struct InlineCtx {
		Register eflags;
		Register edi;
		Register esi;
		Register ebp;
		Register esp;
		Register ebx;
		Register edx;
		Register ecx;
		Register eax;

        inline std::string to_string() {
            return std::format("eax: {:#X}\necx: {:#X}\nedx: {:#X}\nebx: {:#X}\nesp: {:#X}\nebp: {:#X}\nesi: {:#X}\nedi: {:#X}\neflags: {:#X}\n",
                eax.unsigned_integer,
                ecx.unsigned_integer,
                edx.unsigned_integer,
                ebx.unsigned_integer,
                esp.unsigned_integer,
                ebp.unsigned_integer,
                esi.unsigned_integer,
                edi.unsigned_integer,
                eflags.unsigned_integer
            );
        }
    };

    static_assert(sizeof(InlineCtx) == 36);

    template<typename R, typename... A>
    using GenericFuncPtr = R(*)(A...);

    namespace detail {
		
		extern std::vector<utils::JitMemory> JIT_MEMORY;
		extern std::mutex JIT_MEMORY_LOCK;

        template<typename Derived>
        class ReplacementHook {

            template<typename T = Derived>
            using CallbackFuncPtr = decltype(&T::callback);

            static inline auto& orig_ref() {
                static constinit CallbackFuncPtr<Derived> s_func_ptr = nullptr;

                return s_func_ptr;
            }

        public:
            template<typename... Args>
            static inline decltype(auto) original(Args &&... args) {
                return orig_ref()(std::forward<Args>(args)...);
            }

            template<typename R, typename ...A>
            static inline void install_at_func_ptr(GenericFuncPtr<R, A...> ptr) {
                using ArgFuncPtr = decltype(ptr);

                static_assert(std::is_same_v<ArgFuncPtr, CallbackFuncPtr<>>, "Argument pointer type must match callback type!");

                orig_ref() = ptr;

                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }

            static inline void install_at_ptr(uintptr_t ptr) {

                orig_ref() = CallbackFuncPtr<>(ptr);

                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }

            static inline void uninstall() {
                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourDetach(reinterpret_cast<void**>(&orig_ref()), Derived::callback);
                DetourTransactionCommit();
            }
        };

        template<typename Derived>
        class InlineHook {

            template<typename T = Derived>
            using CallbackFuncPtr = decltype(&T::callback);

        public:

            static inline void install_at_ptr(uintptr_t ptr) {
                static_assert(std::is_same_v<void(__cdecl*)(InlineCtx&), CallbackFuncPtr<>>, "Callback function must be void and take an InlineCtx!");

                // Calculate the minimum bytes needed to be backed up, and an upper-bound limit of how many bytes the relocated code could take. (Used for below allocation)
                auto [original_code_len, padded_code_len] = find_suitable_backup_size(ptr);
                
                if (original_code_len < 5) throw std::exception();
				
                // Allocate code for inline handler.
                //auto jit_memory = reinterpret_cast<std::uint8_t*>(VirtualAlloc(NULL, 11 + padded_code_len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
                auto jit_area = utils::JitMemory(11 + padded_code_len + 5);
                auto jit_area_ptr = jit_area.data;

                // Build inline handler.
                *jit_area_ptr = 0x60;
                jit_area_ptr++; // pushad
                *jit_area_ptr = 0x9C;
                jit_area_ptr++; // pushfd
                *jit_area_ptr = 0x54;
                jit_area_ptr++; // push esp
                write_call(jit_area_ptr, Derived::callback);
                jit_area_ptr += 5; // call Derived::callback
                *jit_area_ptr = 0x58;
                jit_area_ptr++; // pop eax
                *jit_area_ptr = 0x9D;
                jit_area_ptr++; // popfd
                *jit_area_ptr = 0x61;
                jit_area_ptr++; // popad
                
                // Attempt to build/relocate the code, and if successful, copy into the trampoline.
                auto relocated = relocate_code(ptr, original_code_len, reinterpret_cast<uintptr_t>(jit_area_ptr)).unwrap();
                std::memcpy(jit_area_ptr, relocated.data(), relocated.size());
                jit_area_ptr += relocated.size();
                write_jmp(jit_area_ptr, reinterpret_cast<void*>(ptr + 5)); jit_area_ptr += 5; // jmp ptr
                // Insert jmp from the source to the inline handler.
                const auto& [old_perm, success] = utils::set_permission(reinterpret_cast<void*>(ptr), original_code_len, utils::Perm::ExecuteReadWrite);
                write_nop(reinterpret_cast<void*>(ptr), original_code_len);
                // Ensure original function has the trampoline area nop'd out.
                write_jmp(reinterpret_cast<void*>(ptr), jit_area_ptr - (11 + relocated.size() + 5));
                utils::set_permission(reinterpret_cast<void*>(ptr), original_code_len, old_perm);

                std::lock_guard<std::mutex> guard(JIT_MEMORY_LOCK);
                JIT_MEMORY.push_back(std::move(jit_area));
            }
        };
    };
};
