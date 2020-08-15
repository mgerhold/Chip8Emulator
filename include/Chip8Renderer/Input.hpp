#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <functional>

class Input {
public:
	using KeyCallbackFunction = std::function<void(uint8_t)>;
public:
	static bool isKeyPressed(uint8_t key) noexcept;
	static void setKeyDownCallback(KeyCallbackFunction function) noexcept;
	static void setKeyUpCallback(KeyCallbackFunction function) noexcept;

private:
	static void glfwKeyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	static std::bitset<0x10> mPressedKeys;
	static KeyCallbackFunction mKeyDownCallback;
	static KeyCallbackFunction mKeyUpCallback;

	friend class Chip8Renderer;
};