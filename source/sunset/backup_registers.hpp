#pragma once
#include <array>
namespace sunset {
	namespace detail {
		namespace x86 {

			/*
			pushad
			pushfd
			sub esp, 16
			movdqu dqword [esp], xmm0
			sub esp, 16
			movdqu dqword [esp], xmm1
			sub esp, 16
			movdqu dqword [esp], xmm2
			sub esp, 16
			movdqu dqword [esp], xmm3
			sub esp, 16
			movdqu dqword [esp], xmm4
			sub esp, 16
			movdqu dqword [esp], xmm5
			sub esp, 16
			movdqu dqword [esp], xmm6
			sub esp, 16
			movdqu dqword [esp], xmm7
			push esp
			*/
			constexpr std::array<std::uint8_t, 99> BACKUP_REGISTERS = { 0x60, 0x9C, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x54 };
			
			/*
			pushad
			pushfd
			push esp
			*/
			constexpr std::array<std::uint8_t, 3> BACKUP_GENERAL_REGISTERS = { 0x60, 0x9C, 0x54 };

			/*
			pop eax
			movdqu xmm7, dqword [esp]
			add esp, 16
			movdqu xmm6, dqword [esp]
			add esp, 16
			movdqu xmm5, dqword [esp]
			add esp, 16
			movdqu xmm4, dqword [esp]
			add esp, 16
			movdqu xmm3, dqword [esp]
			add esp, 16
			movdqu xmm2, dqword [esp]
			add esp, 16
			movdqu xmm1, dqword [esp]
			add esp, 16
			movdqu xmm0, dqword [esp]
			add esp, 16
			popfd
			popad
			*/
			constexpr std::array<std::uint8_t, 99> RESTORE_REGISTERS = { 0x58, 0xF3, 0x0F, 0x6F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x10, 0x9D, 0x61 };

			/*
			pop eax
			popfd
			popad
			*/
			constexpr std::array<std::uint8_t, 3> RESTORE_GENERAL_REGISTERS = { 0x58, 0x9D, 0x61 };
		};
		namespace x64 {

			/*
			push rax
			mov rax, rsp
			add rax, 8
			push rcx
			push rdx
			push rbx
			push rbp
			push rax
			push rsi
			push rdi
			push r8
			push r9
			push r10
			push r11
			push r12
			push r13
			push r14
			push r15
			pushfq
			sub rsp, 16
			movdqu dqword [rsp], xmm0
			sub rsp, 16
			movdqu dqword [rsp], xmm1
			sub rsp, 16
			movdqu dqword [rsp], xmm2
			sub rsp, 16
			movdqu dqword [rsp], xmm3
			sub rsp, 16
			movdqu dqword [rsp], xmm4
			sub rsp, 16
			movdqu dqword [rsp], xmm5
			sub rsp, 16
			movdqu dqword [rsp], xmm6
			sub rsp, 16
			movdqu dqword [rsp], xmm7
			sub rsp, 16
			movdqu dqword [rsp], xmm8
			sub rsp, 16
			movdqu dqword [rsp], xmm9
			sub rsp, 16
			movdqu dqword [rsp], xmm10
			sub rsp, 16
			movdqu dqword [rsp], xmm11
			sub rsp, 16
			movdqu dqword [rsp], xmm12
			sub rsp, 16
			movdqu dqword [rsp], xmm13
			sub rsp, 16
			movdqu dqword [rsp], xmm14
			sub rsp, 16
			movdqu dqword [rsp], xmm15
			mov rcx, rsp
			sub rsp, 8 // This extra sub rsp instruction is here since on x64, the stack must be 16-byte aligned before a function call.
			*/

			// Includes XMM registers
			constexpr std::array<std::uint8_t, 255> BACKUP_REGISTERS = { 0x50, 0x48, 0x89, 0xE0, 0x48, 0x83, 0xC0, 0x08, 0x51, 0x52, 0x53, 0x55, 0x50, 0x56, 0x57, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x9C, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x0F, 0x7F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xEC, 0x10, 0xF3, 0x44, 0x0F, 0x7F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x89, 0xE1, 0x48, 0x83, 0xEC, 0x08 };

			/*
			push rax
			mov rax, rsp
			add rax, 8
			push rcx
			push rdx
			push rbx
			push rbp
			push rax
			push rsi
			push rdi
			push r8
			push r9
			push r10
			push r11
			push r12
			push r13
			push r14
			push r15
			pushfq
			mov rcx, rsp
			sub rsp, 8 // This extra sub rsp instruction is here since on x64, the stack must be 16-byte aligned before a function call.
			*/

			// Omits XMM registers
			constexpr std::array<std::uint8_t, 39> BACKUP_GENERAL_REGISTERS = { 0x50, 0x48, 0x89, 0xE0, 0x48, 0x83, 0xC0, 0x08, 0x51, 0x52, 0x53, 0x55, 0x50, 0x56, 0x57, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x9C, 0x48, 0x89, 0xE1, 0x48, 0x83, 0xEC, 0x08 };

			/*
			add rsp, 8 // This extra add rsp instruction is here since the corresponding backup routine has an extra sub rsp instruction; used to ensure stack alignment.
			movdqu xmm15, dqword [rsp]
			add rsp, 16
			movdqu xmm14, dqword [rsp]
			add rsp, 16
			movdqu xmm13, dqword [rsp]
			add rsp, 16
			movdqu xmm12, dqword [rsp]
			add rsp, 16
			movdqu xmm11, dqword [rsp]
			add rsp, 16
			movdqu xmm10, dqword [rsp]
			add rsp, 16
			movdqu xmm9, dqword [rsp]
			add rsp, 16
			movdqu xmm8, dqword [rsp]
			add rsp, 16
			movdqu xmm7, dqword [rsp]
			add rsp, 16
			movdqu xmm6, dqword [rsp]
			add rsp, 16
			movdqu xmm5, dqword [rsp]
			add rsp, 16
			movdqu xmm4, dqword [rsp]
			add rsp, 16
			movdqu xmm3, dqword [rsp]
			add rsp, 16
			movdqu xmm2, dqword [rsp]
			add rsp, 16
			movdqu xmm1, dqword [rsp]
			add rsp, 16
			movdqu xmm0, dqword [rsp]
			add rsp, 16
			popfq
			pop r15
			pop r14
			pop r13
			pop r12
			pop r11
			pop r10
			pop r9
			pop r8
			pop rdi
			pop rsi
			add rsp, 8
			pop rbp
			pop rbx
			pop rdx
			pop rcx
			pop rax
			*/

			// Includes XMM registers
			constexpr std::array<std::uint8_t, 248> RESTORE_REGISTERS = { 0x48, 0x83, 0xC4, 0x08, 0xF3, 0x44, 0x0F, 0x6F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x44, 0x0F, 0x6F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xBC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xB4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xAC, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x9C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x94, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x8C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0xF3, 0x0F, 0x6F, 0x84, 0x24, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x10, 0x9D, 0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5F, 0x5E, 0x48, 0x83, 0xC4, 0x08, 0x5D, 0x5B, 0x5A, 0x59, 0x58 };

			/*
			add rsp, 8 // This extra add rsp instruction is here since the corresponding backup routine has an extra sub rsp instruction; used to ensure stack alignment.
			popfq
			pop r15
			pop r14
			pop r13
			pop r12
			pop r11
			pop r10
			pop r9
			pop r8
			pop rdi
			pop rsi
			add rsp, 8
			pop rbp
			pop rbx
			pop rdx
			pop rcx
			pop rax
			*/

			// Omits XMM registers
			constexpr std::array<std::uint8_t, 32> RESTORE_GENERAL_REGISTERS = { 0x48, 0x83, 0xC4, 0x08, 0x9D, 0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5F, 0x5E, 0x48, 0x83, 0xC4, 0x08, 0x5D, 0x5B, 0x5A, 0x59, 0x58 };
		};
#if defined(_M_X64)
		namespace arch = x64;
#elif defined(_M_IX86)
		namespace arch = x86;
/*
#elif defined(_M_ARM64)
		namespace arch = aarch64;
*/
#endif
	};
};