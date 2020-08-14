#pragma once

#include <chrono>

class Clock {
public:
	Clock() noexcept;
	float restart() noexcept;
	float getElapsedTime() const noexcept;
private:
	std::chrono::high_resolution_clock mHighResolutionClock;
	std::chrono::high_resolution_clock::time_point mStartTime;
};