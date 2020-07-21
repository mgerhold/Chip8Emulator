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
		Chip8() noexcept;
		bool loadROM(const std::string& filename);
		bool step();
		Chip8Memory<char>& getMemory() noexcept;
		const Chip8Memory<char>& getMemory() const noexcept;

	private:
		std::array<uint8_t, 16> mV; ///< registers V0 to VF
		uint16_t mI; ///< 16 bit memory address register I
		uint16_t mPC; ///< program counter
		Chip8Memory<char> mMemory;
	};

}

