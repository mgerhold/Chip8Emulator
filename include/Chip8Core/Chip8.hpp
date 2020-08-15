/** @file
  * @brief Contains the Chip8::Chip8 class. It resembles the actual emulator.
  */
#pragma once

#include "Chip8Core/Memory.hpp"
#include "Chip8Core/Instruction.hpp"
#include "Chip8Core/OpcodeHandler.hpp"

#include <string>
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <bitset>

namespace Chip8 {

	/**
	 * @brief This class represents the actual CHIP-8 and emulates it.
	 * 
	 * This class is the actual CHIP-8 emulator. It holds holds the memory it needs
	 * and runs the emulation. This class does not render itself and it does not provide
	 * a clock. To run the emulation, one has to call the methods step() and clockTimers()
	 * appropriately.
	*/
	class Chip8 {
	public:
		constexpr static uint16_t ProgramOffset = 0x0200; /**< This is the point in memory where execution starts.*/
		constexpr static size_t DisplayWidth = 64u; /**< The width of the display in pixels.*/
		constexpr static size_t DisplayHeight = 32u; /**< The height of the display in pixels.*/
		using MemoryUnderlyingType = uint8_t; /**< Every addressable piece of memory is stored as this type. */

	public:
		/**
		 * @brief Initializes the CHIP-8 emulator.
		*/
		Chip8() noexcept;

		/**
		 * @brief Resets the emulator to its initial state (registers, program counter,
		 *        memory pointer, timers).
		 * @param alsoResetMemory Whether or not the contents of the memory should be cleared.
		*/
		void reset(bool alsoResetMemory = true) noexcept;

		/**
		 * @brief Loads a CHIP-8 ROM from disk and puts it into memory at the point of execution
		 *        start. See Chip8::Chip8::ProgramOffset.
		 * @param filename The file to open.
		 * @return True if the ROM could be loaded correctly. False otherwise.
		*/
		bool loadROM(const std::string& filename);

		/**
		 * @brief Runs a simulation step for the emulator. This should be called several hundred
		 *        times per second (depending on the program).
		 * @return True on success. False otherwise.
		*/
		bool step();

		/**
		 * @brief Decreases the values of both the delay and the sound timers of the emulator.
		 *        This should be called 60 times per second.
		*/
		void clockTimers() noexcept;

		/**
		 * @brief Returns a reference to the underlying memory for direct read/write access
		 *        if needed.
		 * @see Chip8Memory
		 * @return Reference to the underlying memory.
		*/
		Chip8Memory<MemoryUnderlyingType>& getMemory() noexcept;

		/**
		 * @brief Returns a const reference to the underlying memory for direct read access
		 *        if needed.
		 * @see Chip8Memory
		 * @return Const reference to the underlying memory.
		*/
		const Chip8Memory<MemoryUnderlyingType>& getMemory() const noexcept;

		/**
		 * @brief Reads the value stored in the specified register.
		 * @see setRegister()
		 * @param registerNumber The number of the register (0x0 to 0xF).
		 * @return Value of the register.
		*/
		uint8_t getRegister(uint8_t registerNumber) const noexcept;

		/**
		 * @brief Sets the value for the specified register.
		 * @see getRegister()
		 * @param registerNumber The number of the register (0x0 to 0xF).
		 * @param value The value to be set.
		 * @return 
		*/
		void setRegister(uint8_t registerNumber, uint8_t value) noexcept;

		/**
		 * @brief Returns the current address of the program counter.
		 * @see Chip8::Chip8::ProgramOffset
		 * @return The program counter.
		*/
		uint16_t getProgramCounter() const noexcept;

		/**
		 * @brief Sets the compatibility mode for the emulator. This can be switched
		 *        during execution.
		 *
		 * A few CHIP-8 instructions behave differently on different interpreters. For the
		 * details on this please refer to the <a href="https://en.wikipedia.org/wiki/CHIP-8#cite_note-bitshift-14">
		 * notes on the Wikipedia page</a>.
		 * @param compatibilityMode The compatibility mode (either Chip8::CompatibilityMode::Chip8 or Chip8::CompatibilityMode::SuperChip).
		 * @return 
		*/
		void setCompatibilityMode(CompatibilityMode compatibilityMode) noexcept;

		/**
		 * @brief Returns the compatibility mode the emulator currently runs in.
		 * @return The compatibility mode (either Chip8::CompatibilityMode::Chip8 or Chip8::CompatibilityMode::SuperChip).
		*/
		CompatibilityMode getCompatibilityMode() const noexcept;

		/**
		 * @brief Returns the current address the address pointer (often referred to as I) points to.
		 * @return The address of the address pointer.
		*/
		uint16_t getAddressPointer() const noexcept;

		/**
		 * @brief Returns the current value of the delay timer.
		 * @return The value of the delay timer.
		*/
		uint8_t getDelayTimer() const noexcept;

		/**
		 * @brief Returns the current value of the sound timer.
		 * @return The value of the sound timer.
		*/
		uint8_t getSoundTimer() const noexcept;

		/**
		 * @brief Pushes a return address onto the internal stack. This function usually should
		 *        not get called from outside.
		 * @param returnAddress The return address to push onto the stack.
		*/
		void stackPush(uint16_t returnAddress);

		/**
		 * @brief Pops the return address off the top of the stack and returns it. This function usually should
		 *        not get called from outside.
		 * @return The popped off return address.
		*/
		uint16_t stackPop() noexcept;

		/**
		 * @brief Returns the instruction that will be executed during the next call to step().
		 * @return The next instruction.
		*/
		Instruction getNextInstruction() const;

		/**
		 * @brief Sets or unsets a pixel on the display.
		 * @param x The x coordinate.
		 * @param y The y coordinate.
		 * @param isSet Whether the pixel should be set or unset.
		*/
		void setPixel(size_t x, size_t y, bool isSet);

		/**
		 * @brief Returns the current value of a pixel on the display.
		 * @param x The x coordinate.
		 * @param y The y coordinate.
		 * @return True if the pixel is set, false otherwise.
		*/
		bool getPixel(size_t x, size_t y) const;

		/**
		 * @brief Tells the emulator that a key has been pressed.
		 * @param key The code of the key (0x0 to 0xF).
		*/
		void triggerKeyDown(uint8_t key) noexcept;

		/**
		 * @brief Tells the emulator that a key has been released.
		 * @param key The code of the key (0x0 to 0xF).
		*/
		void triggerKeyUp(uint8_t key) noexcept;

		/**
		 * @brief Returns whether a key is currently being held down.
		 * @param key The code of the key (0x0 to 0xF).
		 * @return True if the key is held down, false otherwise.
		*/
		bool isKeyPressed(uint8_t key) const noexcept;

	private:
		void writeCharacterData();

	private:
		std::array<uint8_t, 16> mV; ///< registers V0 to VF
		uint16_t mI; ///< 16 bit memory address register I
		std::vector<uint16_t> mStack;
		uint16_t mPC; ///< program counter
		uint8_t mDelayTimer;
		uint8_t mSoundTimer;
		Chip8Memory<uint8_t> mMemory;
		CompatibilityMode mCompatibilityMode;
		std::bitset<DisplayWidth * DisplayHeight> mDisplayMemory;
		std::bitset<0x10> mPressedKeys;
		bool mAwaitingKeyPress;
		uint8_t mKeyPressRegisterTarget;

		friend class OpcodeHandler;

		static constexpr uint8_t Characters[] = {
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0x70, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80, // F
		};
	};

}

