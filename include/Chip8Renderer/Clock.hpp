/** @file
  * @brief Contains the Clock helper class. It is used to measure time.
  */
#pragma once

#include <chrono>

/**
 * @brief A helper class for time measuring.
*/
class Clock {
public:
	/**
	 * @brief Creates an instance an immediately starts the clock.
	*/
	Clock() noexcept;

	/**
	 * @brief Restarts the clock.
	 * @return The elapsed time in seconds, right before the restart.
	*/
	float restart() noexcept;

	/**
	 * @brief Returns the elapsed time.
	 * @return Elapsed time in seconds.
	*/
	float getElapsedTime() const noexcept;

private:
	std::chrono::high_resolution_clock mHighResolutionClock;
	std::chrono::high_resolution_clock::time_point mStartTime;
};