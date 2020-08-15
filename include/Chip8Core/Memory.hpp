/** @file
  * @brief Contains the Chip8Memory class. Is resembles the memory of the emulator.
  */
#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>

/**
 * @brief This class template represents the memory for the CHIP-8.
 * @tparam UnderlyingType The data type each memory location will be represented at (usually uint8_t).
*/
template <typename UnderlyingType>
class Chip8Memory {
public:
	/**
	 * @brief Writes a value.
	 * @param address The address to write to.
	 * @param value The value.
	*/
	void write(uint16_t address, UnderlyingType value);

	/**
	 * @brief Reads a value.
	 * @param address The address to read from.
	 * @return The value that has been read.
	*/
	UnderlyingType read(uint16_t address) const;

	/**
	 * @brief Fills the whole memory with zeros.
	*/
	void clear();

	/**
	 * @brief Returns a pointer to the underlying memory. This should usually not
	 *        be used from outside.
	 * @return Pointer to the data.
	*/
	UnderlyingType* data() noexcept;

	/**
	 * @brief Writes the content of the memory to standard output.
	*/
	void dump() const;

private:
	std::array<UnderlyingType, 4096u> mMemory = {};
};

template <typename UnderlyingType>
inline void Chip8Memory<UnderlyingType>::write(uint16_t address, UnderlyingType value) {
	mMemory.at(address) = value;
}

template <typename UnderlyingType>
[[nodiscard]] inline UnderlyingType Chip8Memory<UnderlyingType>::read(uint16_t address) const {
	return mMemory.at(address);
}

template <typename UnderlyingType>
inline void Chip8Memory<UnderlyingType>::clear() {
	mMemory.fill(static_cast<UnderlyingType>(0));
}

template <typename UnderlyingType>
inline UnderlyingType* Chip8Memory<UnderlyingType>::data() noexcept {
	return mMemory.data();
}

template<typename UnderlyingType>
inline void Chip8Memory<UnderlyingType>::dump() const {
	constexpr size_t columns = 32;
	std::cout << "0x" << std::setfill('0') << std::setw(4) << 0u << ": ";
	for (size_t i = 0; i < mMemory.size(); i++) {
		std::cout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << +static_cast<uint8_t>(mMemory.at(i)) << " ";
		if ((i + 1) % columns == 0 && i < mMemory.size() - 1) {			
			std::cout << "\n0x" << std::setfill('0') << std::setw(4) << (i + 1) << ": ";
		}
	}
	std::cout << std::endl;
}
