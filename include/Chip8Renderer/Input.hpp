/** @file
  * @brief Contains the Input class.
  */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <functional>

/**
 * @brief Helper class for handling keyboard input.
*/
class Input {
public:
	/**
	 * @brief This is the type for key press and key release callback functions.
	 * @see setKeyDownCallback(KeyCallbackFunction function)
	 * @see setKeyUpCallback(KeyCallbackFunction function)
	*/
	using KeyCallbackFunction = std::function<void(uint8_t)>;

public:
	/**
	 * @brief Checks if a key is currently pressed.
	 * @param key The key code (0x0 to 0xF).
	 * @return True if the key is pressed, otherwise false.
	*/
	static bool isKeyPressed(uint8_t key) noexcept;

	/**
	 * @brief Set a function that gets called when a key is pressed down.
	 * @param function The callback function.
	*/
	static void setKeyDownCallback(KeyCallbackFunction function) noexcept;

	/**
	 * @brief Set a function that gets called when a key is released.
	 * @param function The callback function.
	*/
	static void setKeyUpCallback(KeyCallbackFunction function) noexcept;

private:
	static void glfwKeyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	static std::bitset<0x10> mPressedKeys;
	static KeyCallbackFunction mKeyDownCallback;
	static KeyCallbackFunction mKeyUpCallback;

	friend class Chip8Renderer;
};