#pragma once

#include <array>
#include <gsl/gsl>
#include <cstdint>
#include <functional>

#include "Chip8Core/Chip8.hpp"
#include "Chip8Core/Instruction.hpp"

namespace Chip8 {

	namespace {

		using InstructionMask = std::array<uint8_t, 2>;

		// helper functions from:
		// https://stackoverflow.com/questions/33484233/how-to-initialize-a-stdarraychar-n-with-a-string-literal-omitting-the-trail
		template <std::size_t N, std::size_t ... Is>
		constexpr std::array<char, N - 1> to_array(const char(&a)[N], std::index_sequence<Is...>) {
			return { {a[Is]...} };
		}

		template<size_t N>
		constexpr std::array<char, N - 1> to_array(const char(&a)[N]) {
			return to_array(a, std::make_index_sequence<N - 1>());
		}

		// helper function to determine if character describes a hexadecimal digit
		constexpr bool isHex(char c) {
			return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
		}

		constexpr InstructionMask instructionMaskFromCharArray(std::array<char, 4> chars) {
			uint8_t first = (isHex(chars.at(0)) ? 0x00 : 0xF0);
			if (!isHex(chars.at(1)))
				first += 0x0F;
			uint8_t second = (isHex(chars.at(2)) ? 0x00 : 0xF0);
			if (!isHex(chars.at(3)))
				second += 0x0F;
			return InstructionMask{ first, second };
		}

		constexpr InstructionMask inverseInstructionMaskFromCharArray(std::array<char, 4> chars) {
			uint8_t first = (isHex(chars.at(0)) ? 0xF0 : 0x00);
			if (isHex(chars.at(1)))
				first += 0x0F;
			uint8_t second = (isHex(chars.at(2)) ? 0xF0 : 0x00);
			if (isHex(chars.at(3)))
				second += 0x0F;
			return InstructionMask{ first, second };
		}

		constexpr uint16_t instructionMaskToUint16(InstructionMask mask) {
			return (mask.at(0) * 0x0100 + mask.at(1));
		}

		constexpr uint8_t hexCharToInt(char c) {
			if (c >= '0' && c <= '9')
				return gsl::narrow<uint8_t>(c - '0');
			else if (c >= 'a' && c <= 'f')
				return gsl::narrow<uint8_t>(10 + c - 'a');
			else if (c >= 'A' && c <= 'F')
				return gsl::narrow<uint8_t>(10 + c - 'A');
			else
				return 0;
		}

		constexpr uint16_t getOpcode(std::array<char, 4> chars) {
			return hexCharToInt(chars.at(3))
				+ hexCharToInt(chars.at(2)) * 0x0010
				+ hexCharToInt(chars.at(1)) * 0x0100
				+ hexCharToInt(chars.at(0)) * 0x1000;
		}

	}

	inline constexpr std::array<std::tuple<const char*, uint16_t, uint16_t, uint16_t>, 35> Opcodes = {
		// name					opcode as uint16_t				opcode mask	as uint16_t															parameter mask as uint16_t
		std::make_tuple("00E0", getOpcode(to_array("00E0")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("00E0"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("00E0")))),
		std::make_tuple("00EE", getOpcode(to_array("00EE")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("00EE"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("00EE")))),
		std::make_tuple("0NNN", getOpcode(to_array("0NNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("0NNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("0NNN")))),
		std::make_tuple("1NNN", getOpcode(to_array("1NNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("1NNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("1NNN")))),
		std::make_tuple("2NNN", getOpcode(to_array("2NNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("2NNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("2NNN")))),
		std::make_tuple("3XNN", getOpcode(to_array("3XNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("3XNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("3XNN")))),
		std::make_tuple("4XNN", getOpcode(to_array("4XNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("4XNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("4XNN")))),
		std::make_tuple("5XY0", getOpcode(to_array("5XY0")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("5XY0"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("5XY0")))),
		std::make_tuple("6XNN", getOpcode(to_array("6XNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("6XNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("6XNN")))),
		std::make_tuple("7XNN", getOpcode(to_array("7XNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("7XNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("7XNN")))),
		std::make_tuple("8XY0", getOpcode(to_array("8XY0")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY0"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY0")))),
		std::make_tuple("8XY1", getOpcode(to_array("8XY1")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY1"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY1")))),
		std::make_tuple("8XY2", getOpcode(to_array("8XY2")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY2"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY2")))),
		std::make_tuple("8XY3", getOpcode(to_array("8XY3")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY3"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY3")))),
		std::make_tuple("8XY4", getOpcode(to_array("8XY4")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY4"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY4")))),
		std::make_tuple("8XY5", getOpcode(to_array("8XY5")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY5"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY5")))),
		std::make_tuple("8XY6", getOpcode(to_array("8XY6")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY6"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY6")))),
		std::make_tuple("8XY7", getOpcode(to_array("8XY7")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XY7"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XY7")))),
		std::make_tuple("8XYE", getOpcode(to_array("8XYE")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("8XYE"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("8XYE")))),
		std::make_tuple("9XY0", getOpcode(to_array("9XY0")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("9XY0"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("9XY0")))),
		std::make_tuple("ANNN", getOpcode(to_array("ANNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("ANNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("ANNN")))),
		std::make_tuple("BNNN", getOpcode(to_array("BNNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("BNNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("BNNN")))),
		std::make_tuple("CXNN", getOpcode(to_array("CXNN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("CXNN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("CXNN")))),
		std::make_tuple("DXYN", getOpcode(to_array("DXYN")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("DXYN"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("DXYN")))),
		std::make_tuple("EX9E", getOpcode(to_array("EX9E")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("EX9E"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("EX9E")))),
		std::make_tuple("EXA1", getOpcode(to_array("EXA1")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("EXA1"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("EXA1")))),
		std::make_tuple("FX07", getOpcode(to_array("FX07")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX07"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX07")))),
		std::make_tuple("FX0A", getOpcode(to_array("FX0A")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX0A"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX0A")))),
		std::make_tuple("FX15", getOpcode(to_array("FX15")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX15"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX15")))),
		std::make_tuple("FX18", getOpcode(to_array("FX18")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX18"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX18")))),
		std::make_tuple("FX1E", getOpcode(to_array("FX1E")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX1E"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX1E")))),
		std::make_tuple("FX29", getOpcode(to_array("FX29")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX29"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX29")))),
		std::make_tuple("FX33", getOpcode(to_array("FX33")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX33"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX33")))),
		std::make_tuple("FX55", getOpcode(to_array("FX55")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX55"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX55")))),
		std::make_tuple("FX65", getOpcode(to_array("FX65")), instructionMaskToUint16(inverseInstructionMaskFromCharArray(to_array("FX65"))), instructionMaskToUint16(instructionMaskFromCharArray(to_array("FX65")))),
	};

}
