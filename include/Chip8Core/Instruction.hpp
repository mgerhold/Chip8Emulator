/** @file
  * @brief Contains the Chip8::Instruction class which holds an instruction that can be executed.
  */
#pragma once

#include <cstddef>
#include <cstdint>

namespace Chip8 {

	/**
	 * @brief This class holds the value of an actual instruction.
	 *
	 * This class provides an interface to access individual components of an instruction. Please
	 * refer to <a href="https://en.wikipedia.org/wiki/CHIP-8#Opcode_table">Wikipedia</a> for a complete
	 * table of all opcodes.
	*/
	class Instruction {
	public:
		/**
		 * @brief Constructs an Instruction instance.
		 * @param value The numeric value of the instruction.
		*/
		Instruction(uint16_t value) noexcept;

		/**
		 * @brief Constructs an Instruction instance.
		 * @param firstByte The upper byte of the instruction.
		 * @param secondByte The lower byte of the instruction.
		*/
		Instruction(uint8_t firstByte, uint8_t secondByte) noexcept;

		/**
		 * @brief Sets the numeric value of the instruction.
		 * @param value The numeric value of the instruction.
		*/
		void setValue(uint16_t value) noexcept;

		/**
		 * @brief Returns the numeric value of the instruction.
		 * @return The numeric value.
		*/
		uint16_t getValue() const noexcept;

		/**
		 * @brief Returns the upper byte of the instruction.
		 * @return The upper byte.
		*/
		std::byte getUpperByte() const noexcept;

		/**
		 * @brief Returns the lower byte of the instruction.
		 * @return The lower byte.
		*/
		std::byte getLowerByte() const noexcept;

		/**
		 * @brief Returns the lowest three nibbles of the instruction. In other words:
		 *		  It returns the numeric value of the instruction masked with 0x0FFF.
		 * @return The lowest three nibbles of the instruction.
		*/
		uint16_t getNNN() const noexcept;

		/**
		 * @brief Returns the lowest two nibbles of the instruction. In other words:
		 *		  It returns the numeric value of the instruction masked with 0x0FFF.
		 * @return The lowest two nibbles of the instruction.
		*/
		uint8_t getNN() const noexcept;

		/**
		 * @brief Returns the lowest nibble of the instruction. In other words:
		 *		  It returns the numeric value of the instruction masked with 0x000F.
		 * @return The lowest nibble of the instruction.
		*/
		uint8_t getN() const noexcept;

		/**
		 * @brief Returns the second nibble of the instruction. In other words:
		 *		  It returns the numeric value of the instruction masked with 0x0F00 shifted
		 *        two places to the right.
		 * @return The second nibble of the instruction.
		*/
		uint8_t getX() const noexcept;

		/**
		 * @brief Returns the third nibble of the instruction. In other words:
		 *		  It returns the numeric value of the instruction masked with 0x00F0 shifted
		 *        one place to the right.
		 * @return The third nibble of the instruction.
		*/
		uint8_t getY() const noexcept;

	private:
		uint16_t mValue;
	};

}
