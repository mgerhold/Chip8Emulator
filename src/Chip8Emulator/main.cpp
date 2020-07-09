#include <iostream>
#include <Chip8Renderer/Chip8Renderer.hpp>

int main() {
	Chip8Renderer renderer;
	if (!renderer.createWindow()) {
		std::cout << "Could not create window!\n";
		exit(-1);
	}
}