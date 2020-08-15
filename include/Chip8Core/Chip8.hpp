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

	class Chip8 {
	public:
		constexpr static uint16_t ProgramOffset = 0x0200; // this is where execution starts
		constexpr static size_t DisplayWidth = 64u;
		constexpr static size_t DisplayHeight = 32u;
		using MemoryUnderlyingType = uint8_t;

	public:
		Chip8() noexcept;
		void reset(bool alsoResetMemory = true) noexcept;
		bool loadROM(const std::string& filename);
		bool step();
		void clockTimers() noexcept;
		Chip8Memory<MemoryUnderlyingType>& getMemory() noexcept;
		const Chip8Memory<MemoryUnderlyingType>& getMemory() const noexcept;
		uint8_t getRegister(uint8_t registerNumber) const noexcept;
		void setRegister(uint8_t registerNumber, uint8_t value) noexcept;		
		uint16_t getProgramCounter() const noexcept;
		void setCompatibilityMode(CompatibilityMode compatibilityMode) noexcept;
		CompatibilityMode getCompatibilityMode() const noexcept;
		uint16_t getAddressPointer() const noexcept;
		uint8_t getDelayTimer() const noexcept;
		uint8_t getSoundTimer() const noexcept;
		void stackPush(uint16_t returnAddress);
		uint16_t stackPop() noexcept;
		Instruction getNextInstruction() const;
		void setPixel(size_t x, size_t y, bool isSet);
		bool getPixel(size_t x, size_t y) const;
		void triggerKeyDown(uint8_t key) noexcept;
		void triggerKeyUp(uint8_t key) noexcept;
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
			0xF0, 0x90, 0xF0, 0x10, 0x90, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80, // F
		};
	};

}

