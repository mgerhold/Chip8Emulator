/** @file
  * @brief Contains the Chip8::OpcodeHandler class, which is responsible for executing instructions.
  */

#pragma once

#include <cstdint>
#include <functional>
#include <map>

/**
 * @brief This namespace holds only the core functionality of the CHIP-8 emulator.
*/
namespace Chip8 {

	class Chip8;
	class Instruction;

	/**
	 * @brief Strongly typed enum that describes the compatibility mode behavior selection. See the
	 * <a href="https://en.wikipedia.org/wiki/CHIP-8#cite_note-bitshift-14">notes on Wikipedia</a> for
	 * the differences in the emulation.
	*/
	enum class CompatibilityMode {
		OriginalChip8,/**< original CHIP-8 behavior */
		SuperChip,/**< SuperChip behavior*/
	};

	/**
	 * @brief This static class is responsible for executing instructions.
	*/
	class OpcodeHandler {
	public:
		/**
		 * @brief Constructs an instance and sets up the random number generator.
		*/
		OpcodeHandler() noexcept;

		/**
		 * @brief Executes an instruction.
		 * @param opcode The CHIP-8 opcode.
		 * @param instruction The instruction to execute.
		 * @param chip8 Reference to a Chip8 instance.
		 * @param compatibilityMode The compatibility mode behavior while executing.
		 * @return True if the execution succeeded, otherwise false.
		*/
		static bool execute(uint16_t opcode, const Instruction& instruction, Chip8& chip8,
			CompatibilityMode compatibilityMode = CompatibilityMode::SuperChip);

	private:
		static void drawSprite(uint8_t x, uint8_t y, uint8_t height, Chip8& chip8);
		static uint8_t generateRandomNumber() noexcept;
	};

}
