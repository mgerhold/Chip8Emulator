#include <iostream>
#include <iomanip>

#include <Chip8Core/Chip8.hpp>
#include <Chip8Renderer/Chip8Renderer.hpp>
#include <Chip8Core/Chip8Opcode.hpp>

int main() {
	auto opcode = Chip8Opcode("DXYN");
	opcode.setValue(OpcodeValue(0xDABC));
	const auto x = opcode.getX().value();
	const auto y = opcode.getY().value();
	const auto n = opcode.getN().value();
	std::cout << "0x" << std::uppercase << std::hex << static_cast<int>(x) << ", 0x" << static_cast<int>(y) << ", 0x" << static_cast<int>(n) << "\n";

	std::cin.get();
}