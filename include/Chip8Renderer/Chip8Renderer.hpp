/** @file
  * @brief Contains the Chip8Renderer class. It is responsible for rendering the state of the emulator
  *        and for managing user input and timing.
  */
#pragma once

#include "Chip8Core/Chip8.hpp"
#include "Chip8Renderer/Clock.hpp"

struct GLFWwindow;
struct GLFWmonitor;

/**
 * @brief This class is not only responsible for rendering the state of the CHIP-8 emulator, but
 *        also for managing the timing and user input.
*/
class Chip8Renderer {
public:
	/**
	 * @brief Constructs an instance.
	 * @param chip8 Reference to a Chip8 instance to render.
	*/
	Chip8Renderer(Chip8::Chip8& chip8) noexcept;

	/**
	 * @brief Free all resources of this instance.
	*/
	void free();

	/**
	 * @brief Creates a new window.
	 * @returns True on success, otherwise false.
	*/
	[[nodiscard]] bool createWindow();

	/**
	 * @brief Enters a loop until the user closes the window.
	*/
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