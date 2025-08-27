#include <Windows.h>
#include <errhandlingapi.h>
#include <DbgHelp.h>
#include <fstream>
#include <sstream>

#include "crash.hpp"
#include "logger.hpp"

constexpr std::uint32_t MS_VC_EXCEPTION = 0x406d1388;

LONG CALLBACK vectored_handler(PEXCEPTION_POINTERS ExceptionInfo) {
    if (ExceptionInfo->ExceptionRecord->ExceptionCode < 0x80000000) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
#if defined(PENTANE_GAME_TARGET_2TVGA)
    if (reinterpret_cast<std::uintptr_t>(ExceptionInfo->ExceptionRecord->ExceptionAddress) == 0x7b5b37) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
#elif defined(PENTANE_GAME_TARGET_2TVG)
    if (reinterpret_cast<std::uintptr_t>(ExceptionInfo->ExceptionRecord->ExceptionAddress) == 0x75d127) {
        return EXCEPTION_CONTINUE_SEARCH;
    }
#endif
    logger::log("=== Exception Caught! ===");
    logger::log_format("Exception Code: 0x{:x}, Fault Address: 0x{:x}", ExceptionInfo->ExceptionRecord->ExceptionCode, reinterpret_cast<std::uintptr_t>(ExceptionInfo->ExceptionRecord->ExceptionAddress));
    
    CONTEXT* ctx = ExceptionInfo->ContextRecord;
    logger::log("Context Record:");
#ifdef _M_X64
    logger::log_format("\trax = 0x{:x}, rbx = 0x{:x}, rcx = 0x{:x}, rdx = 0x{:x}, rsi = 0x{:x}, rdi = 0x{:x}, rbp = 0x{:x}, rsp = 0x{:x}, r8 = 0x{:x}, r9 = 0x{:x}, r10 = 0x{:x}, r11 = 0x{:x}, r12 = 0x{:x}, r13 = 0x{:x}, r14 = 0x{:x}, r15 = 0x{:x}, rip = 0x{:x}", ctx->Rax, ctx->Rbx, ctx->Rcx, ctx->Rdx, ctx->Rsi, ctx->Rdi, ctx->Rbp, ctx->Rsp, ctx->R8, ctx->R9, ctx->R10, ctx->R11, ctx->R12, ctx->R13, ctx->R14, ctx->R15, ctx->Rip);
#else
    logger::log_format("\teax = 0x{:x}, ebx = 0x{:x}, ecx = 0x{:x}, edx = 0x{:x}, esi = 0x{:x}, edi = 0x{:x}, ebp = 0x{:x}, esp = 0x{:x}, eip = 0x{:x}", ctx->Eax, ctx->Ebx, ctx->Ecx, ctx->Edx, ctx->Esi, ctx->Edi, ctx->Ebp, ctx->Esp, ctx->Eip);
#endif
    logger::log("Stack Trace:");
    
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    SymInitialize(process, nullptr, TRUE);

    STACKFRAME64 stack = {};
#ifdef _M_X64
    DWORD machine_type = IMAGE_FILE_MACHINE_AMD64;
    stack.AddrPC.Offset = ctx->Rip;
    stack.AddrFrame.Offset = ctx->Rbp;
    stack.AddrStack.Offset = ctx->Rsp;
#else
    DWORD machine_type = IMAGE_FILE_MACHINE_I386;
    stack.AddrPC.Offset = ctx->Eip;
    stack.AddrFrame.Offset = ctx->Ebp;
    stack.AddrStack.Offset = ctx->Esp;
#endif
    stack.AddrPC.Mode = AddrModeFlat;
    stack.AddrFrame.Mode = AddrModeFlat;
    stack.AddrStack.Mode = AddrModeFlat;

    for (int i = 0; i < 32; i++) {
        if (StackWalk64(machine_type, process, thread, &stack, ctx, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr) == 0) {
            break;
        }
        if (stack.AddrPC.Offset == 0) {
            break;
        }

        std::uint8_t symbol_buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME]{};
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbol_buffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;
        if (SymFromAddr(process, stack.AddrPC.Offset, &displacement, symbol) != 0) {
            logger::log_format("\tEntry: {}, Symbol: {} + 0x{:x}, Address: 0x{:x}", i, symbol->Name, displacement, stack.AddrPC.Offset);
        }
        else {
            logger::log_format("\tEntry: {}, Address: 0x{:x}", i, stack.AddrPC.Offset);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}


void crash_handler::install() {
    AddVectoredExceptionHandler(1, vectored_handler);
}