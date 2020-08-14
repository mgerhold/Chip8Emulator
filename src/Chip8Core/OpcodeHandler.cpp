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

		std::cout << "executing " << std::setw(4) << std::setfill('0') << std::hex << instruction.getValue() << "\n\t";
		switch (opcode) {
			case 0x0000: // 0NNN
				std::cout << "0NNN with NNN = " << instruction.getNNN() << "\n";
				// Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.
				// TODO: log that this opcode is purposely not implemented
				break;
			case 0x00E0: // 00E0
				std::cout << "00E0\n";
				// TODO: implementation
				break;
			case 0x00EE: // 00EE
				std::cout << "00EE\n";
				// Returns from a subroutine.
				chip8.mPC = chip8.stackPop();
				break;
			case 0x1000: // 1NNN
				std::cout << "1NNN with NNN = " << instruction.getNNN() << "\n";
				// Jumps to address NNN.
				chip8.mPC = instruction.getNNN();
				break;
			case 0x2000: // 2NNN
				std::cout << "2NNN with NNN = " << instruction.getNNN() << "\n";
				// Calls subroutine at NNN.
				chip8.stackPush(chip8.getProgramCounter());
				chip8.mPC = instruction.getNNN();
				break;
			case 0x3000: // 3XNN
				std::cout << "3XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				// Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) == instruction.getNN())
					chip8.mPC += 0x2;
				break;
			case 0x4000: // 4XNN
				std::cout << "4XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				// Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) != instruction.getNN())
					chip8.mPC += 0x2;
				break;
			case 0x5000: // 5XY0
				std::cout << "5XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) == chip8.getRegister(instruction.getY()))
					chip8.mPC += 0x2;
				break;
			case 0x6000: // 6XNN
				std::cout << "6XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				// Sets VX to NN.
				chip8.setRegister(instruction.getX(), instruction.getNN());
				break;
			case 0x7000: // 7XNN
				std::cout << "7XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				// Adds NN to VX. (Carry flag is not changed)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) + instruction.getNN());
				break;
			case 0x8000: // 8XY0
				std::cout << "8XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Sets VX to the value of VY.
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()));
				break;
			case 0x8001: // 8XY1
				std::cout << "8XY1 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Sets VX to VX or VY. (Bitwise OR operation)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) | chip8.getRegister(instruction.getY()));
				break;
			case 0x8002: // 8XY2
				std::cout << "8XY2 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Sets VX to VX and VY. (Bitwise AND operation)
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) & chip8.getRegister(instruction.getY()));
				break;
			case 0x8003: // 8XY3
				std::cout << "8XY3 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Sets VX to VX xor VY.
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) ^ chip8.getRegister(instruction.getY()));
				break;
			case 0x8004: // 8XY4
				std::cout << "8XY4 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				if (static_cast<uint16_t>(chip8.getRegister(instruction.getX())) + static_cast<uint16_t>(chip8.getRegister(instruction.getY())) > 0xFF)
					chip8.setRegister(0xF, 0x1);
				else
					chip8.setRegister(0xF, 0x0);
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) + chip8.getRegister(instruction.getY()));
				break;
			case 0x8005: // 8XY5
				std::cout << "8XY5 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				if (static_cast<uint16_t>(chip8.getRegister(instruction.getX())) > static_cast<uint16_t>(chip8.getRegister(instruction.getY())))
					chip8.setRegister(0xF, 0x1); // no borrow
				else
					chip8.setRegister(0xF, 0x0); // borrow
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getX()) - chip8.getRegister(instruction.getY()));
				break;
			case 0x8006: // 8XY6
				std::cout << "8XY6 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
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
				std::cout << "8XY7 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				if (chip8.getRegister(instruction.getY()) > chip8.getRegister(instruction.getX()))
					chip8.setRegister(0xF, 0x1); // no borrow
				else
					chip8.setRegister(0xF, 0x0); // borrow
				chip8.setRegister(instruction.getX(), chip8.getRegister(instruction.getY()) - chip8.getRegister(instruction.getX()));
				break;
			case 0x800E: // 8XYE
				std::cout << "8XYE with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
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
				std::cout << "9XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				// Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
				if (chip8.getRegister(instruction.getX()) != chip8.getRegister(instruction.getY()))
					chip8.mPC += 0x2;
				break;
			case 0xA000: // ANNN
				std::cout << "ANNN with NNN = " << +instruction.getNNN() << "\n";
				// Sets I to the address NNN.
				chip8.mI = instruction.getNNN();
				break;
			case 0xB000: // BNNN
				std::cout << "BNNN with NNN = " << +instruction.getNNN() << "\n";
				// Jumps to the address NNN plus V0.
				chip8.mPC = chip8.getRegister(0x0) + instruction.getNNN();
				break;
			case 0xC000: // CXNN
				std::cout << "CXNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
				chip8.setRegister(instruction.getX(), generateRandomNumber() & instruction.getNN());
				break;
			case 0xD000: // DXYN
				std::cout << "DXYN with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << " and N = " << instruction.getN() << "\n";
				// TODO: implementation
				break;
			case 0xE09E: // EX9E
				std::cout << "EX9E with X = " << +instruction.getX() << "\n";
				// TODO: implementation
				break;
			case 0xE0A1: // EXA1
				std::cout << "EXA1 with X = " << +instruction.getX() << "\n";
				// TODO: implementation
				break;
			case 0xF007: // FX07
				std::cout << "FX07 with X = " << +instruction.getX() << "\n";
				// Sets VX to the value of the delay timer.
				chip8.setRegister(instruction.getX(), chip8.mDelayTimer);
				break;
			case 0xF00A: // FX0A
				std::cout << "FX0A with X = " << +instruction.getX() << "\n";
				// TODO: implementation
				break;
			case 0xF015: // FX15
				std::cout << "FX15 with X = " << +instruction.getX() << "\n";
				// 	Sets the delay timer to VX.
				chip8.mDelayTimer = chip8.getRegister(instruction.getX());
				break;
			case 0xF018: // FX18
				std::cout << "FX18 with X = " << +instruction.getX() << "\n";
				// Sets the sound timer to VX.
				chip8.mSoundTimer = chip8.getRegister(instruction.getX());
				break;
			case 0xF01E: // FX1E
				std::cout << "FX1E with X = " << +instruction.getX() << "\n";
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
				std::cout << "FX29 with X = " << +instruction.getX() << "\n";
				// TODO: implementation
				break;
			case 0xF033: // FX33
				std::cout << "FX33 with X = " << +instruction.getX() << "\n";
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
				std::cout << "FX55 with X = " << +instruction.getX() << "\n";
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
				std::cout << "FX65 with X = " << +instruction.getX() << "\n";
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

	uint8_t OpcodeHandler::generateRandomNumber() noexcept {
		return gsl::narrow<uint8_t>(std::rand() % 255);
	}

}
