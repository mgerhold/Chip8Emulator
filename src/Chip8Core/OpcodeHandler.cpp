#include "Chip8Core/OpcodeHandler.hpp"

#include <iostream>
#include <iomanip>

#include "Chip8Core/Chip8.hpp"
#include "Chip8Core/Instruction.hpp"

namespace Chip8 {

	bool OpcodeHandler::execute(uint16_t opcode, const Instruction& instruction, Chip8& /*chip8*/) {
		std::cout << "executing " << std::setw(4) << std::setfill('0') << std::hex << instruction.getValue() << "\n\t";
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
			case 0x8001: // 8XY1
				std::cout << "8XY1 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8002: // 8XY2
				std::cout << "8XY2 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8003: // 8XY3
				std::cout << "8XY3 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8004: // 8XY4
				std::cout << "8XY4 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8005: // 8XY5
				std::cout << "8XY5 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8006: // 8XY6
				std::cout << "8XY6 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x8007: // 8XY7
				std::cout << "8XY7 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x800E: // 8XYE
				std::cout << "8XYE with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0x9000: // 9XY0
				std::cout << "9XY0 with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << "\n";
				break;
			case 0xA000: // ANNN
				std::cout << "ANNN with NNN = " << +instruction.getNNN() << "\n";
				break;
			case 0xB000: // BNNN
				std::cout << "BNNN with NNN = " << +instruction.getNNN() << "\n";
				break;
			case 0xC000: // CXNN
				std::cout << "CXNN with X = " << +instruction.getX() << " and NN = " << +instruction.getNN() << "\n";
				break;
			case 0xD000: // DXYN
				std::cout << "DXYN with X = " << +instruction.getX() << " and Y = " << +instruction.getY() << " and N = " << instruction.getN() << "\n";
				break;
			case 0xE09E: // EX9E
				std::cout << "EX9E with X = " << +instruction.getX() << "\n";
				break;
			case 0xE0A1: // EXA1
				std::cout << "EXA1 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF00A: // FX0A
				std::cout << "FX0A with X = " << +instruction.getX() << "\n";
				break;
			case 0xF015: // FX15
				std::cout << "FX15 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF018: // FX18
				std::cout << "FX18 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF01E: // FX1E
				std::cout << "FX1E with X = " << +instruction.getX() << "\n";
				break;
			case 0xF029: // FX29
				std::cout << "FX29 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF033: // FX33
				std::cout << "FX33 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF055: // FX55
				std::cout << "FX55 with X = " << +instruction.getX() << "\n";
				break;
			case 0xF065: // FX65
				std::cout << "FX65 with X = " << +instruction.getX() << "\n";
				break;
			default:
				std::cout << "Critical Error: Opcode " << opcode << " (not implemented)\n";
				return false;				
		}
		return true;
	}

}
