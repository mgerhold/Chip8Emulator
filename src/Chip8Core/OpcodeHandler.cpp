#include "Chip8Core/OpcodeHandler.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <gsl/gsl>

#include "Chip8Core/Chip8.hpp"
#include "Chip8Core/Instruction.hpp"

namespace Chip8 {
	OpcodeHandler::OpcodeHandler() noexcept {
		std::srand(gsl::narrow<unsigned int>(std::time(nullptr)));
	}

	bool OpcodeHandler::execute(uint16_t opcode, const Instruction& instruction, Chip8& chip8, CompatibilityMode compatibilityMode) {
		// important note: the program counter will already be incremented upon entering this function!

		// std::cout << "executing " << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << instruction.getValue() << "\n\t";
		switch (opcode) {
			case 0x0000: // 0NNN
				// Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.
				std::cout << "Info: Opcode 0NNN is purposely not implemented.\n";
				break;
			case 0x00E0: // 00E0
				// Clears the screen.
				chip8.mDisplayMemory.reset();
				break;
			case 0x00EE: // 00EE
				// Returns from a subroutine.
				chip8.mPC = chip8.stackPop();
				break;
			case 0x1000: // 1NNN
				// Jumps to address NNN.
				chip8.mPC = instruction.getNNN();
				break;
			case 0x2000: // 2NNN
				// Calls subroutine at NNN.
				chip8.stackPush(chip8.getProgramCounter());
				chip8.mPC = instruction.getNNN();
				break;
			case 0x3000: // 3XNN
				// Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) == instruction.getNN())
					chip8.mPC += 0x2;
				break;
			case 0x4000: // 4XNN
				// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) != instruction.getNN())
					chip8.mPC += 0x2;
				break;
			case 0x5000: // 5XY0
				// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) == chip8.getRegister(instruction.getY()))
					chip8.mPC += 0x2;
				break;
			case 0x6000: // 6XNN
				// Sets VX to NN.
				chip8.setRegister(instruction.getX(), instruction.getNN());
				break;
			case 0x7000: // 7XNN
				// Adds NN to VX. (Carry flag is not changed)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) + instruction.getNN());
				break;
			case 0x8000: // 8XY0
				// Sets VX to the value of VY.
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()));
				break;
			case 0x8001: // 8XY1
				// Sets VX to VX or VY. (Bitwise OR operation)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) | chip8.getRegister(instruction.getY()));
				break;
			case 0x8002: // 8XY2
				// Sets VX to VX and VY. (Bitwise AND operation)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) & chip8.getRegister(instruction.getY()));
				break;
			case 0x8003: // 8XY3
				// Sets VX to VX xor VY.
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) ^ chip8.getRegister(instruction.getY()));
				break;
			case 0x8004: // 8XY4
				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				if (static_cast<uint16_t>(chip8.getRegister(instruction.getX())) + static_cast<uint16_t>(chip8.getRegister(instruction.getY())) > 0xFF)
					chip8.setRegister(0xF, 0x1);
				else
					chip8.setRegister(0xF, 0x0);
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) + chip8.getRegister(instruction.getY()));
				break;
			case 0x8005: // 8XY5
				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				if (static_cast<uint16_t>(chip8.getRegister(instruction.getX())) > static_cast<uint16_t>(chip8.getRegister(instruction.getY())))
					chip8.setRegister(0xF, 0x1); // no borrow
				else
					chip8.setRegister(0xF, 0x0); // borrow
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) - chip8.getRegister(instruction.getY()));
				break;
			case 0x8006: // 8XY6
				// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
				//
				// Additional information:
				// CHIP-8's opcodes 8XY6 and 8XYE (the bit shift instructions), which were in fact undocumented opcodes
				// in the original interpreter, shifted the value in the register VY and stored the result in VX. 
				// The CHIP-48 and SCHIP implementations instead ignored VY, and simply shifted VX.
				if (compatibilityMode == CompatibilityMode::OriginalChip8) {
					chip8.setRegister(0xF, chip8.getRegister(instruction.getY()) & 0x1);
					chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()) >> 1);
				} else if (compatibilityMode == CompatibilityMode::SuperChip) {
					chip8.setRegister(0xF, chip8.getRegister(instruction.getX()) & 0x1);
					chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) >> 1);
				} else {
					std::cout << "Critical Error: Unknown compatibility mode.\n";
					return false;
				}
				break;
			case 0x8007: // 8XY7
				// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				if (chip8.getRegister(instruction.getY()) > chip8.getRegister(instruction.getX()))
					chip8.setRegister(0xF, 0x1); // no borrow
				else
					chip8.setRegister(0xF, 0x0); // borrow
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()) - chip8.getRegister(instruction.getX()));
				break;
			case 0x800E: // 8XYE
				// 	Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
				//
				// For additional information see: 8XY6
				if (compatibilityMode == CompatibilityMode::OriginalChip8) {
					if (chip8.getRegister(instruction.getY()) & 0b1000'0000)
						chip8.setRegister(0xF, 0x1);
					else
						chip8.setRegister(0xF, 0x0);
					chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()) << 1);
				} else if (compatibilityMode == CompatibilityMode::SuperChip) {
					if (chip8.getRegister(instruction.getX()) & 0b1000'0000)
						chip8.setRegister(0xF, 0x1);
					else
						chip8.setRegister(0xF, 0x0);
					chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) << 1);
				} else {
					std::cout << "Critical Error: Unknown compatibility mode.\n";
					return false;
				}
				break;
			case 0x9000: // 9XY0
				// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) != chip8.getRegister(instruction.getY()))
					chip8.mPC += 0x2;
				break;
			case 0xA000: // ANNN
				// Sets I to the address NNN.
				chip8.mI = instruction.getNNN();
				break;
			case 0xB000: // BNNN
				// Jumps to the address NNN plus V0.
				chip8.mPC = chip8.getRegister(0x0) + instruction.getNNN();
				break;
			case 0xC000: // CXNN
				// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
				chip8.setRegister(instruction.getX(), generateRandomNumber() & instruction.getNN());
				break;
			case 0xD000: // DXYN
				// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
				// Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t change
				// after the execution of this instruction. As described above, VF is set to 1 if any screen pixels
				// are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
				drawSprite(chip8.getRegister(instruction.getX()), chip8.getRegister(instruction.getY()), instruction.getN(), chip8);
				break;
			case 0xE09E: // EX9E
				// Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction
				// is a jump to skip a code block)
				if (chip8.isKeyPressed(chip8.getRegister(instruction.getX())))
					chip8.mPC += 2;
				break;
			case 0xE0A1: // EXA1
				// Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction
				// is a jump to skip a code block)
				if (!chip8.isKeyPressed(chip8.getRegister(instruction.getX())))
					chip8.mPC += 2;
				break;
			case 0xF007: // FX07
				// Sets VX to the value of the delay timer.
				chip8.setRegister(instruction.getX(), chip8.mDelayTimer);
				break;
			case 0xF00A: // FX0A
				// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until
				// next key event)
				chip8.mAwaitingKeyPress = true;
				chip8.mKeyPressRegisterTarget = instruction.getX();
				break;
			case 0xF015: // FX15
				// 	Sets the delay timer to VX.
				chip8.mDelayTimer = chip8.getRegister(instruction.getX());
				break;
			case 0xF018: // FX18
				// Sets the sound timer to VX.
				chip8.mSoundTimer = chip8.getRegister(instruction.getX());
				break;
			case 0xF01E: // FX1E
				// Adds VX to I. VF is not affected.
				//
				// Additional information:
				// Most CHIP-8 interpreters' FX1E instructions do not affect VF, with one exception:
				// The CHIP-8 interpreter for the Commodore Amiga sets VF to 1 when there is a range
				// overflow (I+VX>0xFFF), and to 0 when there isn't.[13] The only known game that depends
				// on this behavior is Spacefight 2091! while at least one game, Animal Race, depends on
				// VF not being affected.
				// -- The behavior of the Amiga will not be available in this implementation.
				chip8.mI += chip8.getRegister(instruction.getX());
				break;
			case 0xF029: // FX29
				// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal)
				// are represented by a 4x5 font.
				chip8.mI = (5u * chip8.getRegister(instruction.getX()));
				break;
			case 0xF033: // FX33
				// Stores the binary-coded decimal representation of VX, with the most significant of three
				// digits at the address in I, the middle digit at I plus 1, and the least significant digit
				// at I plus 2. (In other words, take the decimal representation of VX, place the hundreds
				// digit in memory at location in I, the tens digit at location I+1, and the ones digit at
				// location I+2.)
				chip8.getMemory().write(chip8.getAddressPointer() + 0x0, chip8.getRegister(instruction.getX()) / 100);
				chip8.getMemory().write(chip8.getAddressPointer() + 0x1, (chip8.getRegister(instruction.getX()) % 100) / 10);
				chip8.getMemory().write(chip8.getAddressPointer() + 0x2, chip8.getRegister(instruction.getX()) % 10);
				break;
			case 0xF055: // FX55
				// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is
				// increased by 1 for each value written, but I itself is left unmodified.
				//
				// Additional information:
				// In the original CHIP-8 implementation, and also in CHIP-48, I is left incremented after
				// this instruction had been executed. In SCHIP, I is left unmodified.
				if (compatibilityMode == CompatibilityMode::OriginalChip8) {
					for (uint8_t i = 0; i <= instruction.getX(); i++) {
						chip8.getMemory().write(chip8.mI++, chip8.getRegister(i));
					}
				} else if (compatibilityMode == CompatibilityMode::SuperChip) {
					for (uint8_t i = 0; i <= instruction.getX(); i++) {
						chip8.getMemory().write(chip8.mI + i, chip8.getRegister(i));
					}
				} else {
					std::cout << "Critical Error: Unknown compatibility mode.\n";
					return false;
				}
				break;
			case 0xF065: // FX65
				// Fills V0 to VX (including VX) with values from memory starting at address I. The
				// offset from I is increased by 1 for each value written, but I itself is left unmodified.
				//
				// For additional information see: FX55
				if (compatibilityMode == CompatibilityMode::OriginalChip8) {
					for (uint8_t i = 0; i <= instruction.getX(); i++) {
						chip8.setRegister(i, chip8.getMemory().read(chip8.mI++));
					}
				} else if (compatibilityMode == CompatibilityMode::SuperChip) {
					for (uint8_t i = 0; i <= instruction.getX(); i++) {
						chip8.setRegister(i, chip8.getMemory().read(chip8.mI + i));
					}
				} else {
					std::cout << "Critical Error: Unknown compatibility mode.\n";
					return false;
				}
				break;
			default:
				std::cout << "Critical Error: Opcode " << opcode << " (not implemented)\n";
				return false;				
		}
		return true;
	}

	void OpcodeHandler::drawSprite(uint8_t x, uint8_t y, uint8_t height, Chip8& chip8) {
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
		// Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn�t
		// change after the execution of this instruction. As described above, VF is set to 1 if any
		// screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn�t happen
		Ensures(height <= 0xF);
		bool collision = false;
		for (uint8_t row = 0x0; row < height; ++row) {
			for (uint8_t col = 0x0; col < 0x8; ++col) {
				bool oldPixel = chip8.getPixel(static_cast<size_t>(x) + col, static_cast<size_t>(y) + row);
				uint16_t memoryPosition = chip8.getAddressPointer() + row;
				uint8_t mask = (0x1 << (0x7 - col));
				bool newPixel = ((chip8.getMemory().read(memoryPosition) & mask) != 0x0);
				if (newPixel) {
					if (oldPixel)
						collision = true;
					chip8.setPixel(static_cast<size_t>(x) + col, static_cast<size_t>(y) + row, !oldPixel);
				}
			}
		}
		chip8.setRegister(0xF, collision ? 0x1 : 0x0);
	}

	uint8_t OpcodeHandler::generateRandomNumber() noexcept {
		return gsl::narrow<uint8_t>(std::rand() % 255);
	}

}
