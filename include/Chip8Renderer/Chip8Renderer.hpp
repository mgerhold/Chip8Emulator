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
	void renderDisplay() const;
	void renderImGui();
	void centerWindow(GLFWwindow* window, GLFWmonitor* monitor);
	void drawUnitQuad() const;

private:
	GLFWwindow* mWindow;
	Chip8::Chip8& mChip8;
	Clock mTimerClock;
	Clock mUpdateClock;
	float mScaleFactor;
	float mPixelColor[3];
	float mBackgroundColor[3];
	float mClearColor[3];
	bool mRunning;
	bool mStepping;
	std::string mMessage;
	Chip8::Instruction mLastInstruction;
	float mLastTimerClockTime;
	float mLastUpdateClockTime;
	int mUpdatesPerSecond;
};