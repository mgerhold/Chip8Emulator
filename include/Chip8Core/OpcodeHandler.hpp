#pragma once

#include <cstdint>
#include <functional>
#include <map>

namespace Chip8 {

	class Chip8;
	class Instruction;

	enum class CompatibilityMode {
		OriginalChip8,
		SuperChip,
	};

	class OpcodeHandler {
	public:
		OpcodeHandler() noexcept;

		static bool execute(uint16_t opcode, const Instruction& instruction, Chip8& chip8,
			CompatibilityMode compatibilityMode = CompatibilityMode::SuperChip);
	private:
		static uint8_t generateRandomNumber() noexcept;
	};

}
