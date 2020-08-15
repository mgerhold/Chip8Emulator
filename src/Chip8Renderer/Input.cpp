#include "Chip8Renderer/Input.hpp"

#include <iostream>
#include <unordered_map>
#include <gsl/gsl>

std::bitset<0x10> Input::mPressedKeys = std::bitset<0x10>();
Input::KeyCallbackFunction Input::mKeyDownCallback = nullptr;
Input::KeyCallbackFunction Input::mKeyUpCallback = nullptr;

bool Input::isKeyPressed(uint8_t key) noexcept {
	return mPressedKeys[key];
}

void Input::setKeyDownCallback(KeyCallbackFunction function) noexcept {
	mKeyDownCallback = function;
}

void Input::setKeyUpCallback(KeyCallbackFunction function) noexcept {
	mKeyUpCallback = function;
}

void Input::glfwKeyCallbackFunction(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/) {
	const static std::unordered_map<uint8_t, int> keyMapping = {
		{ gsl::narrow<uint8_t>(0x1), GLFW_KEY_1 },
		{ gsl::narrow<uint8_t>(0x2), GLFW_KEY_2 },
		{ gsl::narrow<uint8_t>(0x3), GLFW_KEY_3 },
		{ gsl::narrow<uint8_t>(0xC), GLFW_KEY_4 },
		{ gsl::narrow<uint8_t>(0x4), GLFW_KEY_Q },
		{ gsl::narrow<uint8_t>(0x5), GLFW_KEY_W },
		{ gsl::narrow<uint8_t>(0x6), GLFW_KEY_E },
		{ gsl::narrow<uint8_t>(0xD), GLFW_KEY_R },
		{ gsl::narrow<uint8_t>(0x7), GLFW_KEY_A },
		{ gsl::narrow<uint8_t>(0x8), GLFW_KEY_S },
		{ gsl::narrow<uint8_t>(0x9), GLFW_KEY_D },
		{ gsl::narrow<uint8_t>(0xE), GLFW_KEY_F },
		{ gsl::narrow<uint8_t>(0xA), GLFW_KEY_Z },
		{ gsl::narrow<uint8_t>(0x0), GLFW_KEY_X },
		{ gsl::narrow<uint8_t>(0xB), GLFW_KEY_C },
		{ gsl::narrow<uint8_t>(0xF), GLFW_KEY_V },
	};
	for (const auto& [chip8key, glfwKeyCode] : keyMapping) {
		if (key == glfwKeyCode) {
			if (mPressedKeys[chip8key] && action == GLFW_RELEASE && mKeyUpCallback)
				mKeyUpCallback(chip8key);
			else if (!mPressedKeys[chip8key] && action == GLFW_PRESS && mKeyDownCallback)
				mKeyDownCallback(chip8key);
			mPressedKeys[chip8key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
			break;
		}
	}
}
