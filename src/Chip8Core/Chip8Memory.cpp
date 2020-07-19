#include "Chip8Core/Chip8Memory.hpp"

#include <stdexcept>

void Chip8Memory::write(size_t address, std::byte value) {
	mMemory.at(address) = value;
}

[[nodiscard]] std::byte Chip8Memory::read(size_t address) const {
	return mMemory.at(address);
}
