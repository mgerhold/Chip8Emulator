#include "Chip8Core/OpcodeHandler.hpp"

#include <iostream>
#include <iomanip>

#include "Chip8Core/Chip8.hpp"
#include "Chip8Core/Instruction.hpp"

namespace Chip8 {

	bool OpcodeHandler::execute(uint16_t opcode, const Instruction& instruction, Chip8& /*chip8*/) {
		std::cout << "executing " << std::setw(4) << std::setfill('0') << std::hex << instruction.getValue() << "\n";
		switch (opcode) {
			case 0x0000: // 0NNN
				std::cout << "0NNN with NNN = " << instruction.getNNN() << "\n";
				break;
			case 0x00E0: // 00E0
				std::cout << "00E0\n";
				break;
			case 0x00EE: // 00EE
				std::cout << "00EE\n";
				break;
			case 0x1000: // 1NNN
				std::cout << "1NNN with NNN = " << instruction.getNNN() << "\n";
				break;
			case 0x2000: // 2NNN
				std::cout << "2NNN with NNN = " << instruction.getNNN() << "\n";
				break;
			case 0x3000: // 3XNN
				std::cout << "3XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				break;
			case 0x4000: // 4XNN
				std::cout << "4XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				break;
			case 0x5000: // 5XY0
				std::cout << "5XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x6000: // 6XNN
				std::cout << "6XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				break;
			case 0x7000: // 7XNN
				std::cout << "7XNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				break;
			case 0x8000: // 8XY0
				std::cout << "8XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			default:
				std::cout << "Unknown opcode " << opcode << " (not implemented)\n";
				return false;				
		}
		return true;
	}

}
