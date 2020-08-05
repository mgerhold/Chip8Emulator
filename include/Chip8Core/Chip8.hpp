#pragma once

#include "Chip8Core/Memory.hpp"
#include "Chip8Core/Instruction.hpp"

#include <string>
#include <array>
#include <cstdint>
#include <cstddef>

namespace Chip8 {

	class Chip8 {
	public:
		constexpr static uint16_t ProgramOffset = 0x0200; // this is where execution starts
		using MemoryUnderlyingType = uint8_t;

	public:
		Chip8() noexcept;
		bool loadROM(const std::string& filename);
		bool step();
		Chip8Memory<MemoryUnderlyingType>& getMemory() noexcept;
		const Chip8Memory<MemoryUnderlyingType>& getMemory() const noexcept;
		uint8_t getRegister(uint8_t registerNumber) const noexcept;
		void setRegister(uint8_t registerNumber, uint8_t value) noexcept;

	private:
		std::array<uint8_t, 16> mV; ///< registers V0 to VF
		uint16_t mI; ///< 16 bit memory address register I
		uint16_t mPC; ///< program counter
		Chip8Memory<uint8_t> mMemory;
	};

}

