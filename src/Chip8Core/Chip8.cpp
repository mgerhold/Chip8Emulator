#include "Chip8Core/Chip8.hpp"

#ifdef _MSC_VER
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif
#include <iostream>
#include <fstream>
#include <cassert>

#include <gsl/gsl>

#include "Chip8Core/Instruction.hpp"
#include "Chip8Core/Opcodes.hpp"

namespace Chip8 {

    Chip8::Chip8() noexcept
        : mV({}), mI(0), mPC(ProgramOffset), mDelayTimer(0x0), mSoundTimer(0x0)
        , mCompatibilityMode(CompatibilityMode::SuperChip)
    {}

    void Chip8::reset(bool alsoResetMemory) noexcept {
        if (alsoResetMemory)
            mMemory.clear();
        for (uint8_t i = 0; i <= 0xF; ++i)
            setRegister(i, 0x0);
        mPC = ProgramOffset;
        mI = 0x0;
        mDelayTimer = 0x0;
        mSoundTimer = 0x0;
        mCompatibilityMode = CompatibilityMode::SuperChip;
    }

    bool Chip8::loadROM(const std::string& filename) {
        if (!fs::exists(filename)) {
            return false;
        } else {
            std::basic_ifstream<uint8_t> file;
            file.open(filename, std::ios::in | std::ios::binary);
            if (file.good()) {
                reset();
#ifdef _MSC_VER
#pragma warning(disable: 26481)
#endif
                file.read(mMemory.data() + 0x0200, 0x1000 - 0x0200);
#ifdef _MSC_VER
#pragma warning(default: 26481)
#endif
            } else {
                return false;
            }
            return true;
        }
    }

    bool Chip8::step() {
        // check if program counter is valid
        if (mPC < 0x1000) {
            // read next instruction and increase program counter
            const Instruction instruction(mMemory.read(mPC), mMemory.read(mPC + 1));
            mPC += 2;

            // evaluate instruction
            bool instructionFound = false;
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
            for (auto& [name, opcode, opcodeMask, parameterMask] : Opcodes) {
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif
                if ((instruction.getValue() & opcodeMask) == opcode) {
                    instructionFound = true;
                    if (!OpcodeHandler::execute(opcode, instruction, *this, mCompatibilityMode))
                        return false;
                    if (mDelayTimer > 0x0)
                        mDelayTimer--;
                    if (mSoundTimer > 0x0)
                        mSoundTimer--;
                    break;
                }
            }
            if (!instructionFound)
                std::cout << "Warning: Instruction 0x"
                    << std::setw(4) << std::setfill('0') << std::hex << std::uppercase
                    << instruction.getValue() << " could not be evaluated (unknown opcode).\n";
            return true;
        } else {
            std::cout << "end of program reached\n";
            return false;
        }
    }

    Chip8Memory<Chip8::MemoryUnderlyingType>& Chip8::getMemory() noexcept {
        return mMemory;
    }

    const Chip8Memory<Chip8::MemoryUnderlyingType>& Chip8::getMemory() const noexcept {
        return mMemory;
    }

    uint8_t Chip8::getRegister(uint8_t registerNumber) const noexcept  {        
        return mV.at(registerNumber);
    }

    void Chip8::setRegister(uint8_t registerNumber, uint8_t value) noexcept {
        mV.at(registerNumber) = value;
    }

    uint16_t Chip8::getProgramCounter() const noexcept {
        return mPC;
    }

    void Chip8::setCompatibilityMode(CompatibilityMode compatibilityMode) noexcept {
        mCompatibilityMode = compatibilityMode;
    }

    CompatibilityMode Chip8::getCompatibilityMode() const noexcept {
        return mCompatibilityMode;
    }

    uint16_t Chip8::getAddressPointer() const noexcept {
        return mI;
    }

    uint8_t Chip8::getDelayTimer() const noexcept {
        return mDelayTimer;
    }

    void Chip8::stackPush(uint16_t returnAddress) {
        mStack.push_back(returnAddress);
    }

    uint16_t Chip8::stackPop() noexcept {
        Ensures(mStack.size() > 0);
        const uint16_t result = mStack.back();
        mStack.pop_back();
        return result;
    }

    Instruction Chip8::getNextInstruction() const {
        return Instruction(mMemory.read(mPC), mMemory.read(mPC + 1));
    }

    void Chip8::setPixel(size_t x, size_t y, bool isSet) {
        mDisplayMemory[x + DisplayWidth * y] = isSet;
    }
    bool Chip8::getPixel(size_t x, size_t y) const {
        return mDisplayMemory[x + DisplayWidth * y];
    }
}

