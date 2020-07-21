#include <iostream>
#include <iomanip>

#include <Chip8Core/Chip8.hpp>
#include <Chip8Renderer/Chip8Renderer.hpp>
#include <Chip8Core/Opcodes.hpp>

int main() {
	Chip8::Chip8 chip8;
	if (!chip8.loadROM("roms/programs/Life [GV Samways, 1980].ch8"))
		std::cout << "Could not open file!\n";
	else
		chip8.getMemory().dump();

	for (auto& [name, opcode, opcodeMask, parameterMask] : Chip8::Opcodes) {		
		std::cout << name << ", 0x" << std::setfill('0') << std::setw(4) << std::hex << opcode << ", 0x" << std::setw(4) << opcodeMask << ", 0x" << std::setw(4) << parameterMask << "\n";
	}

	while (chip8.step());

	std::cin.get();
}