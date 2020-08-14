#pragma once

#include "Chip8Core/Chip8.hpp"
#include "Chip8Renderer/Clock.hpp"

struct GLFWwindow;
struct GLFWmonitor;

class Chip8Renderer {
public:
	Chip8Renderer(Chip8::Chip8& chip8) noexcept;
	void free();
	[[nodiscard]] bool createWindow();
	void startRenderLoop();

private:
	void renderImGui();
	void centerWindow(GLFWwindow* window, GLFWmonitor* monitor);

private:
	GLFWwindow* mWindow;
	Chip8::Chip8& mChip8;
	Clock mClock;
	float mPixelSize;
	float mPixelColor[3];
	float mBackgroundColor[3];
	bool mRunning;
	std::string mMessage;
	Chip8::Instruction mLastInstruction;
	float mLastElapsedTime;
};
