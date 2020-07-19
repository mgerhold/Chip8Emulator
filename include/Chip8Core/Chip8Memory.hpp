#pragma once

#include <cstddef>
#include <array>

class Chip8Memory {
public:
	void write(size_t address, std::byte value);
	std::byte read(size_t address) const;

private:
	std::array<std::byte, 4096u> mMemory = {};
};