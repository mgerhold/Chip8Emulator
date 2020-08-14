#include "..\..\include\Chip8Renderer\Clock.hpp"

Clock::Clock() noexcept {
	restart();
}

float Clock::restart() noexcept  {
	auto result = getElapsedTime();
	mStartTime = mHighResolutionClock.now();
	return result;
}

float Clock::getElapsedTime() const noexcept {
	auto duration = mHighResolutionClock.now() - mStartTime;
	return (static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()) / 1000.0f);
}
