#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <optional>
#include <format>

#include "err.hpp"

#define ZYDIS_STATIC_BUILD
#include <Zydis/Decoder.h>
#include <Zydis/Zydis.h>

namespace sunset {
	namespace detail {
		enum class CodeRelocError {
			// Failed to calculate an absolute address.
			FailedAbsoluteAddressCalc,
			// Failed to convert a ZydisDecodedInstruction to a ZydisEncoderRequest.
			FailedDecodedInstrToEncoderRequest,
			// Failed to encode an instruction.
			FailedEncodeInstr
		};
	}
};

namespace std
{
	template<> struct formatter<sunset::detail::CodeRelocError> : public std::formatter<std::string>
	{
		constexpr auto parse(std::format_parse_context& ctx) {
			return ctx.begin();
		}

		auto format(const sunset::detail::CodeRelocError& p, std::format_context& fc) const
		{
			switch (p)
			{
			case sunset::detail::CodeRelocError::FailedAbsoluteAddressCalc:
				return std::format_to(fc.out(), "FailedAbsoluteAddressCalc");
			case sunset::detail::CodeRelocError::FailedDecodedInstrToEncoderRequest:
				return std::format_to(fc.out(), "FailedDecodedInstrToEncoderRequest");
			case sunset::detail::CodeRelocError::FailedEncodeInstr:
				return std::format_to(fc.out(), "FailedEncodeInstr");
			default:
				return std::format_to(fc.out(), "");
			}
		}
	};
};

namespace sunset {
	namespace detail {

		inline Result<std::vector<std::uint8_t>, CodeRelocError> relocate_code(std::uintptr_t source, std::size_t source_size, std::uintptr_t dest) {
			ZydisDecoder decoder{};
			ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_STACK_WIDTH_32);
			
			ZydisDecodedInstruction instruction{};
			ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};

			ZydisEncoderRequest encoder_request{};

			std::ptrdiff_t source_offset = 0;
			std::ptrdiff_t dest_offset = 0;
			std::vector<std::uint8_t> output;
			
			while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, reinterpret_cast<const void*>(source + source_offset), source_size - source_offset, &instruction, operands))) {
				// Iterate over all operands, checking if each one is EIP-relative and patching the address accordingly.
				for (std::size_t index = 0; index < instruction.operand_count; index++) {
					auto& elem = operands[index];
					if (elem.type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
						if (elem.imm.is_relative) {
							ZyanU64 absolute_address = 0;
							if (!ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&instruction, &elem, static_cast<ZyanU64>(source + source_offset), &absolute_address))) {
								return Err(CodeRelocError::FailedAbsoluteAddressCalc);
							}
							elem.imm.value.u = absolute_address;
							elem.imm.is_relative = false;
						}
					}
					if (elem.type == ZYDIS_OPERAND_TYPE_MEMORY) {
						if (elem.mem.base == ZYDIS_REGISTER_EIP) {
							ZyanU64 absolute_address = 0;
							if (!ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&instruction, &elem, static_cast<ZyanU64>(source + source_offset), &absolute_address))) {
								return Err(CodeRelocError::FailedAbsoluteAddressCalc);
							}
							elem.mem.base = ZYDIS_REGISTER_NONE;
							elem.mem.disp.value = absolute_address;
						}
					}
				}

				std::size_t old_size = output.size();

				// Reserve space in the vector for the encoded instruction.
				output.resize(old_size + ZYDIS_MAX_INSTRUCTION_LENGTH);

				// Convert the decoded instruction to an encoder request.
				if (!ZYAN_SUCCESS(ZydisEncoderDecodedInstructionToEncoderRequest(&instruction, operands, instruction.operand_count_visible, &encoder_request))) {
					return Err(CodeRelocError::FailedDecodedInstrToEncoderRequest);
				}

				// Encode the instruction into the output vector.
				ZyanUSize encoded_size = ZYDIS_MAX_INSTRUCTION_LENGTH;
				if (!ZYAN_SUCCESS(ZydisEncoderEncodeInstructionAbsolute(&encoder_request, output.data() + dest_offset, &encoded_size, static_cast<ZyanU64>(dest + dest_offset)))) {
					return Err(CodeRelocError::FailedEncodeInstr);
				}

				// Shrink the vector to fit.
				output.resize(old_size + encoded_size);

				// Increment the offset.
				source_offset += instruction.length;
				dest_offset += encoded_size;
			}
			return Ok(std::move(output));
		}
		
		inline std::size_t get_instruction_len(std::uintptr_t ptr) {
			ZydisDecoder decoder{};
			ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_STACK_WIDTH_32);

			ZydisDecodedInstruction instruction{};
			ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};
			if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, reinterpret_cast<const void*>(ptr), ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction, operands))) {
				return 0;
			}
			return instruction.length;
		}

		inline std::pair<std::size_t, std::size_t> find_suitable_backup_size(std::uintptr_t base) {
			ZydisDecoder decoder{};
			ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_STACK_WIDTH_32);

			ZydisDecodedInstruction instruction{};
			ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]{};

			std::ptrdiff_t offset = 0;
			std::size_t padded = 0;

			while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, reinterpret_cast<const void*>(base + offset), ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction, operands))) {
				if (offset >= 5) {
					break;
				}
				offset += instruction.length;
				padded += ZYDIS_MAX_INSTRUCTION_LENGTH;
			}
			return { offset, padded };
		}
	};
};
