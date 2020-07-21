#pragma once

#include <cstdint>
#include <functional>
#include <map>

namespace Chip8 {

	class Chip8;
	class Instruction;

	class OpcodeHandler {
	public:
		static bool execute(uint16_t opcode, const Instruction& instruction, Chip8& chip8);

	};

}
