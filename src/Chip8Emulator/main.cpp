#include <iostream>
#include <iomanip>

#include <Chip8Renderer/Chip8Renderer.hpp>
#include <Chip8Core/Chip8.hpp>
#include <Chip8Core/Opcodes.hpp>

int main() {
	Chip8::Chip8 chip8;
	if (!chip8.loadROM("roms/test.ch8"))
		std::cout << "Could not open file!\n";
	chip8.setPixel(0, 0, true);
	chip8.setPixel(chip8.DisplayWidth - 1, 0, true);
	chip8.setPixel(0, chip8.DisplayHeight - 1, true);

	Chip8Renderer renderer(chip8);
	if (!renderer.createWindow()) {
		std::cout << "Failed to open window\n";
	}
	renderer.startRenderLoop();
	renderer.free();
}