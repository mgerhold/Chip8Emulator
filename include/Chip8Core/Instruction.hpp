#pragma once

#include <cstddef>
#include <cstdint>

namespace Chip8 {

	class Instruction {
	public:
		Instruction(uint16_t value) noexcept;
		Instruction(uint8_t firstByte, uint8_t secondByte) noexcept;
		void setValue(uint16_t value) noexcept;
		uint16_t getValue() const noexcept;
		std::byte getUpperByte() const noexcept;
		std::byte getLowerByte() const noexcept;
		uint16_t getNNN() const noexcept;
		uint8_t getNN() const noexcept;
		uint8_t getN() const noexcept;
		uint8_t getX() const noexcept;
		uint8_t getY() const noexcept;

	private:
		uint16_t mValue;
	};

}
