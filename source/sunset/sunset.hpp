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
#include "backup_registers.hpp"


#define DefineReplacementHook(name) \
struct name : public sunset::detail::ReplacementHook<name>

#define DefineInlineHook(name) \
struct name : public sunset::detail::InlineHook<name>

#define DefineExtendedInlineHook(name) \
struct name : public sunset::detail::ExInlineHook<name>

namespace sunset {
    namespace detail {
        enum class InlineHookError {
            // The user-set address does not have enough free space (>= 5 Bytes) for a jmp instruction.
            NotEnoughSpaceToInsertJump,
            // The callback function is too far away from the inline handler to use a 6-Byte call-indirect instruction.
            CallbackTooFarFromInlineHandler,
            // The original function is too far away from the inline handler to use 5-Byte jmp instructions.
            OriginalCodeTooFarFromInlineHandler
        };
    }
};

namespace std
{
    template<> struct formatter<sunset::detail::InlineHookError> : public std::formatter<std::string>
    {
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const sunset::detail::InlineHookError& p, std::format_context& fc) const
        {
            switch (p)
            {
            case sunset::detail::InlineHookError::NotEnoughSpaceToInsertJump:
                return std::format_to(fc.out(), "NotEnoughSpaceToInsertJump");
            case sunset::detail::InlineHookError::CallbackTooFarFromInlineHandler:
                return std::format_to(fc.out(), "CallbackTooFarFromInlineHandler");
            case sunset::detail::InlineHookError::OriginalCodeTooFarFromInlineHandler:
                return std::format_to(fc.out(), "OriginalCodeTooFarFromInlineHandler");
            default:
                return std::format_to(fc.out(), "");
            }
        }
    };
};

namespace sunset {

    namespace utils {

        template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> constexpr bool is_pow_two(T n) {
            return n != 0 && (n & (n - 1)) == 0;
        }

        template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> constexpr T align_up(T num, T align) {
            return (num + align - 1) & ~(align - 1);
        }

        template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr> constexpr T align_down(T num, T align) {
            return num & ~(align - 1);
        }

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
        inline Result<Perm, DWORD> set_permission(T ptr, size_t size, Perm perm) {
            Perm old_perm = Perm::None;
            bool success = static_cast<bool>(VirtualProtect(reinterpret_cast<void*>(ptr), size, static_cast<DWORD>(perm), reinterpret_cast<DWORD*>(&old_perm)));
            if (success) {
                return Ok(std::move(old_perm));
            }
            else {
                return Err(GetLastError());
            }
        }

        inline std::uintptr_t get_main_load_address() {
            return reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
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
            inline JitMemory(void* _data, std::size_t size) {
                data = reinterpret_cast<std::uint8_t*>(_data);
                len = size;
            }
        public:
            inline JitMemory(std::size_t size) {
                data = reinterpret_cast<std::uint8_t*>(VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
                len = size;
            }
            inline JitMemory() {
                data = nullptr;
                len = 0;
            }
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
            static inline JitMemory from_raw_parts(void* data, std::size_t len) {
                return std::move(JitMemory(data, len));
            }
        };

        inline Option<JitMemory> allocate_near(std::uintptr_t address, std::size_t len, std::size_t max_distance = (std::numeric_limits<std::int32_t>::max)()) {
            SYSTEM_INFO info{};
            GetSystemInfo(&info);

            MEMORY_BASIC_INFORMATION mbi{};
            std::uintptr_t aligned_address = align_down(address, static_cast<std::size_t>(info.dwAllocationGranularity));

            // Iterate backwards towards the start of the available address space from the given `address` argument.
            std::uintptr_t current_address = aligned_address;
            while (current_address > reinterpret_cast<std::uintptr_t>(info.lpMinimumApplicationAddress) && static_cast<std::size_t>(std::llabs(current_address - address)) < max_distance) {
                VirtualQueryEx(GetCurrentProcess(), reinterpret_cast<void*>(current_address), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
                if (mbi.State == MEM_FREE) {
                    void* memory = VirtualAlloc(reinterpret_cast<void*>(current_address), len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                    if (memory != nullptr) {
                        return Some(JitMemory::from_raw_parts(memory, len));
                    }
                }
                current_address -= info.dwAllocationGranularity;
            }

            // Iterate forwards towards the end of the available address space from the given `address` argument.
            current_address = aligned_address;
            while (current_address < reinterpret_cast<std::uintptr_t>(info.lpMaximumApplicationAddress) && static_cast<std::size_t>(std::llabs(current_address - address)) < max_distance) {
                VirtualQueryEx(GetCurrentProcess(), reinterpret_cast<void*>(current_address), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
                if (mbi.State == MEM_FREE) {
                    void* memory = VirtualAlloc(reinterpret_cast<void*>(current_address), len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                    if (memory != nullptr) {
                        return Some(JitMemory::from_raw_parts(memory, len));
                    }
                }
                current_address += info.dwAllocationGranularity;
            }

            return None;
        }
    };

    namespace inst {

        inline bool jmp(void* src, void* dst) {
            std::uintptr_t destination_address = reinterpret_cast<std::uintptr_t>(dst);
            std::uintptr_t source_address = reinterpret_cast<std::uintptr_t>(src);
            std::intptr_t relative_address = destination_address - source_address - 5;
            if (relative_address < (std::numeric_limits<std::int32_t>::min)() || relative_address > (std::numeric_limits<std::int32_t>::max)()) {
                return false;
            }
            auto restore = utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(source_address) = 0xE9;
            *reinterpret_cast<std::int32_t*>(source_address + 1) = static_cast<std::int32_t>(relative_address);
            utils::set_permission(src, 5, restore);
            return true;
        }

        inline bool call(void* src, void* dst) {
            std::uintptr_t destination_address = reinterpret_cast<std::uintptr_t>(dst);
            std::uintptr_t source_address = reinterpret_cast<std::uintptr_t>(src);
            std::intptr_t relative_address = destination_address - source_address - 5;
            if (relative_address < (std::numeric_limits<std::int32_t>::min)() || relative_address > (std::numeric_limits<std::int32_t>::max)()) {
                return false;
            }
            auto restore = utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(source_address) = 0xE8;
            *reinterpret_cast<std::int32_t*>(source_address + 1) = static_cast<std::int32_t>(relative_address);
            utils::set_permission(src, 5, restore);
            return true;
        }

#if defined(_M_X64)
        inline bool call_indirect(void* src, void* dst) {
            std::uintptr_t destination_address = reinterpret_cast<std::uintptr_t>(dst);
            std::uintptr_t source_address = reinterpret_cast<std::uintptr_t>(src);
            std::intptr_t relative_address = destination_address - source_address - 6;
            if (relative_address < (std::numeric_limits<std::int32_t>::min)() || relative_address >(std::numeric_limits<std::int32_t>::max)()) {
                return false;
            }
            auto restore = utils::set_permission(src, 6, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(source_address) = 0xFF;
            *reinterpret_cast<std::uint8_t*>(source_address + 1) = 0x15;
            *reinterpret_cast<std::int32_t*>(source_address + 2) = static_cast<std::int32_t>(relative_address);
            utils::set_permission(src, 6, restore);
            return true;
        }
#elif defined(_M_IX86)
        inline bool call_indirect(void* src, void* dst) {
            std::uintptr_t destination_address = reinterpret_cast<std::uintptr_t>(dst);
            std::uintptr_t source_address = reinterpret_cast<std::uintptr_t>(src);
            auto restore = utils::set_permission(src, 6, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(source_address) = 0xFF;
            *reinterpret_cast<std::uint8_t*>(source_address + 1) = 0x15;
            *reinterpret_cast<std::uint32_t*>(source_address + 2) = static_cast<std::uint32_t>(destination_address);
            utils::set_permission(src, 6, restore);
            return true;
        }
#endif

        inline void push_u32(void* src, std::uint32_t dst) {
            auto restore = utils::set_permission(src, 5, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(src) = 0x68;
            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(src) + 1) = dst;
            utils::set_permission(src, 5, restore).unwrap();
        }

        inline void push_u8(void* src, std::uint8_t dst) {
            auto restore = utils::set_permission(src, 2, utils::Perm::ExecuteReadWrite).unwrap();
            *reinterpret_cast<std::uint8_t*>(src) = 0x6A;
            *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(src) + 1) = dst;
            utils::set_permission(src, 2, restore).unwrap();
        }

        inline void nop(void* addr, std::size_t code_size) {
            auto original_protection = utils::set_permission(addr, code_size, utils::Perm::ExecuteReadWrite).unwrap();
            std::memset(reinterpret_cast<void*>(addr), 0x90, code_size);
            utils::set_permission(addr, code_size, original_protection).unwrap();
        }
    };

    union XMMRegister {
        float f32[4];
        double f64[2];
        int8_t i8[16];
        uint8_t u8[16];
        int16_t i16[8];
        uint16_t u16[8];
        int32_t i32[4];
        uint32_t u32[4];
        int64_t i64[2];
        uint64_t u64[2];
    };

    static_assert(sizeof(XMMRegister) == 16);

#ifdef _M_X64
    union Register {
        void* pointer;
        std::uint64_t unsigned_integer;
        std::int64_t signed_integer;
        double floating_point;
    };
    
    static_assert(sizeof(Register) == 8);

    struct InlineCtx {
        Register rflags;
        Register r15;
        Register r14;
        Register r13;
        Register r12;
        Register r11;
        Register r10;
        Register r9;
        Register r8;
        Register rdi;
        Register rsi;
        Register rsp;
        Register rbp;
        Register rbx;
        Register rdx;
        Register rcx;
        Register rax;
    };

    struct InlineCtxEx {
        XMMRegister xmm15;
        XMMRegister xmm14;
        XMMRegister xmm13;
        XMMRegister xmm12;
        XMMRegister xmm11;
        XMMRegister xmm10;
        XMMRegister xmm9;
        XMMRegister xmm8;
        XMMRegister xmm7;
        XMMRegister xmm6;
        XMMRegister xmm5;
        XMMRegister xmm4;
        XMMRegister xmm3;
        XMMRegister xmm2;
        XMMRegister xmm1;
        XMMRegister xmm0;
        Register rflags;
        Register r15;
        Register r14;
        Register r13;
        Register r12;
        Register r11;
        Register r10;
        Register r9;
        Register r8;
        Register rdi;
        Register rsi;
        Register rsp;
        Register rbp;
        Register rbx;
        Register rdx;
        Register rcx;
        Register rax;
    };

    static_assert(sizeof(InlineCtx) == 136);
    static_assert(sizeof(InlineCtxEx) == 392);
#endif

#ifdef _M_IX86
    union Register {
        void* pointer;
        std::uint32_t unsigned_integer;
        std::int32_t signed_integer;
        float floating_point;
    };
    static_assert(sizeof(Register) == 4);

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
    };

    struct InlineCtxEx {
        XMMRegister xmm7;
        XMMRegister xmm6;
        XMMRegister xmm5;
        XMMRegister xmm4;
        XMMRegister xmm3;
        XMMRegister xmm2;
        XMMRegister xmm1;
        XMMRegister xmm0;
        Register eflags;
        Register edi;
        Register esi;
        Register ebp;
        Register esp;
        Register ebx;
        Register edx;
        Register ecx;
        Register eax;
    };

    static_assert(sizeof(InlineCtx) == 36);
    static_assert(sizeof(InlineCtxEx) == 168);
#endif

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

            static inline void install_at_ptr(std::uintptr_t ptr) {

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

            static inline Result<bool, InlineHookError> install_at_ptr(std::uintptr_t ptr) {
                static_assert(std::is_same_v<void(__cdecl*)(InlineCtx&), CallbackFuncPtr<>>, "Callback function must be void and take an InlineCtx!");

                // Calculate the minimum bytes needed to be backed up, and an upper-bound limit of how many bytes the relocated code could take. (Used for below allocation)
                auto [original_code_len, padded_code_len] = find_suitable_backup_size(ptr);
                
                if (original_code_len < MINIMUM_OVERWRITE) {
                    return Err(InlineHookError::NotEnoughSpaceToInsertJump);
                }

                // Allocate code for inline handler.
                // The size of the JIT memory block should be able to account for:
                //  - A pointer to Derived::callback (sizeof(void*))
                //  - The register backup routine
                //  - A 6-byte indirect call instruction to call Derived::callback (6)
                //  - The register restore routine
                //  - A 5-byte jump instruction to return to the original code (5)
                const std::size_t jit_area_len = arch::BACKUP_GENERAL_REGISTERS.size() + arch::RESTORE_GENERAL_REGISTERS.size() + 6 + padded_code_len + 5 + sizeof(void*);
#ifdef _M_X64
                // On x64, if we can't find any free space nearby, we have to panic.
                utils::JitMemory jit_area = utils::allocate_near(ptr, jit_area_len).unwrap();
#else
                // But on x86, we can reliably fall back to a plain ol' VirtualAlloc.
                utils::JitMemory jit_area = utils::allocate_near(ptr, jit_area_len).unwrap_or(utils::JitMemory(jit_area_len));
#endif
                // Write the callback address at the start of the inline handler, so we can use a call-indirect instruction to get there.
                std::uint8_t* jit_area_ptr = jit_area.data;
                *reinterpret_cast<std::uintptr_t*>(jit_area_ptr) = reinterpret_cast<std::uintptr_t>(Derived::callback);
                jit_area_ptr += sizeof(void*);

                // Build inline handler.
                std::memcpy(jit_area_ptr, arch::BACKUP_GENERAL_REGISTERS.data(), arch::BACKUP_GENERAL_REGISTERS.size());
                jit_area_ptr += arch::BACKUP_GENERAL_REGISTERS.size();
                // The address of the callback is held at the start of the JIT memory, so we'll use jit_area.data here.
                if (!inst::call_indirect(jit_area_ptr, jit_area.data)) {
                    // This should be infallible since it will literally only be a few hundred bytes away tops...
                    return Err(InlineHookError::CallbackTooFarFromInlineHandler);
                }
                jit_area_ptr += 6;
                std::memcpy(jit_area_ptr, arch::RESTORE_GENERAL_REGISTERS.data(), arch::RESTORE_GENERAL_REGISTERS.size());
                jit_area_ptr += arch::RESTORE_GENERAL_REGISTERS.size();

                // Attempt to build/relocate the code, and if successful, copy into the trampoline.
                std::vector<std::uint8_t> relocated = relocate_code(ptr, original_code_len, reinterpret_cast<std::uintptr_t>(jit_area_ptr)).unwrap();
                std::memcpy(jit_area_ptr, relocated.data(), relocated.size());
                jit_area_ptr += relocated.size();

                // Write the jmp from the inline handler back to the original function.
                if (!inst::jmp(jit_area_ptr, reinterpret_cast<void*>(ptr + original_code_len))) {
                    return Err(InlineHookError::OriginalCodeTooFarFromInlineHandler);
                }
                jit_area_ptr += 5;
                
                // Ensure original function has the trampoline area nop'd out before inserting the jmp from the source to the inline handler (jmp ptr).
                inst::nop(reinterpret_cast<void*>(ptr), original_code_len);
                if (!inst::jmp(reinterpret_cast<void*>(ptr), jit_area.data + sizeof(void*))) {
                    return Err(InlineHookError::OriginalCodeTooFarFromInlineHandler);
                }
                
                std::lock_guard<std::mutex> guard(JIT_MEMORY_LOCK);
                JIT_MEMORY.push_back(std::move(jit_area));
                
                return Ok(true);
            }
        };

        // FIXME: Add support for extended inline hooks on x86.
        template<typename Derived>
        class ExInlineHook {

            template<typename T = Derived>
            using CallbackFuncPtr = decltype(&T::callback);

        public:

            static inline Result<bool, InlineHookError> install_at_ptr(std::uintptr_t ptr) {
                static_assert(std::is_same_v<void(__cdecl*)(InlineCtxEx&), CallbackFuncPtr<>>, "Callback function must be void and take an InlineCtxEx!");

                // Calculate the minimum bytes needed to be backed up, and an upper-bound limit of how many bytes the relocated code could take. (Used for below allocation)
                auto [original_code_len, padded_code_len] = find_suitable_backup_size(ptr);

                if (original_code_len < MINIMUM_OVERWRITE) {
                    return Err(InlineHookError::NotEnoughSpaceToInsertJump);
                }

                // Allocate code for inline handler.
                // The size of the JIT memory block should be able to account for:
                //  - A pointer to Derived::callback (sizeof(void*))
                //  - The register backup routine
                //  - A 6-byte indirect call instruction to call Derived::callback (6)
                //  - The register restore routine
                //  - A 5-byte jump instruction to return to the original code (5)
                const std::size_t jit_area_len = arch::BACKUP_REGISTERS.size() + arch::RESTORE_REGISTERS.size() + 6 + padded_code_len + 5 + sizeof(void*);
#ifdef _M_X64
                // On x64, if we can't find any free space nearby, we have to panic.
                utils::JitMemory jit_area = utils::allocate_near(ptr, jit_area_len).unwrap();
#else
                // But on x86, we can reliably fall back to a plain ol' VirtualAlloc.
                utils::JitMemory jit_area = utils::allocate_near(ptr, jit_area_len).unwrap_or(utils::JitMemory(jit_area_len));
#endif
                // Write the callback address at the start of the inline handler, so we can use a call-indirect instruction to get there.
                std::uint8_t* jit_area_ptr = jit_area.data;
                *reinterpret_cast<std::uintptr_t*>(jit_area_ptr) = reinterpret_cast<std::uintptr_t>(Derived::callback);
                jit_area_ptr += sizeof(void*);

                // Build inline handler.
                std::memcpy(jit_area_ptr, arch::BACKUP_REGISTERS.data(), arch::BACKUP_REGISTERS.size());
                jit_area_ptr += arch::BACKUP_REGISTERS.size();
                // The address of the callback is held at the start of the JIT memory, so we'll use jit_area.data here.
                if (!inst::call_indirect(jit_area_ptr, jit_area.data)) {
                    // This should be infallible since it will literally only be a few hundred bytes away tops...
                    return Err(InlineHookError::CallbackTooFarFromInlineHandler);
                }
                jit_area_ptr += 6;
                std::memcpy(jit_area_ptr, arch::RESTORE_REGISTERS.data(), arch::RESTORE_REGISTERS.size());
                jit_area_ptr += arch::RESTORE_REGISTERS.size();

                // Attempt to build/relocate the code, and if successful, copy into the trampoline.
                std::vector<std::uint8_t> relocated = relocate_code(ptr, original_code_len, reinterpret_cast<std::uintptr_t>(jit_area_ptr)).unwrap();
                std::memcpy(jit_area_ptr, relocated.data(), relocated.size());
                jit_area_ptr += relocated.size();

                // Write the jmp from the inline handler back to the original function.
                if (!inst::jmp(jit_area_ptr, reinterpret_cast<void*>(ptr + original_code_len))) {
                    return Err(InlineHookError::OriginalCodeTooFarFromInlineHandler);
                }
                jit_area_ptr += 5;

                // Ensure original function has the trampoline area nop'd out before inserting the jmp from the source to the inline handler (jmp ptr).
                inst::nop(reinterpret_cast<void*>(ptr), original_code_len);
                if (!inst::jmp(reinterpret_cast<void*>(ptr), jit_area.data + sizeof(void*))) {
                    return Err(InlineHookError::OriginalCodeTooFarFromInlineHandler);
                }

                std::lock_guard<std::mutex> guard(JIT_MEMORY_LOCK);
                JIT_MEMORY.push_back(std::move(jit_area));

                return Ok(true);
            }
        };
    };
};
