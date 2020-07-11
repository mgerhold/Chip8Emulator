#pragma once

class Chip8Renderer {
public:
	Chip8Renderer() noexcept;
	[[nodiscard]] bool createWindow();
};
