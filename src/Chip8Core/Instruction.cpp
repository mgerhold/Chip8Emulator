#include "Chip8Core/Instruction.hpp"
#include <gsl/gsl>

namespace Chip8 {

    Instruction::Instruction(uint16_t value) noexcept
        : mValue(value)
    {}

    Instruction::Instruction(uint8_t firstByte, uint8_t secondByte) noexcept
        : mValue(firstByte * 0x0100 + secondByte)
    {}

    void Instruction::setValue(uint16_t value) noexcept {
        mValue = value;
    }

    uint16_t Instruction::getValue() const noexcept {
        return mValue;
    }

    std::byte Instruction::getUpperByte() const noexcept {
        return static_cast<std::byte>((mValue & 0xFF00) / 0x0100);
    }

    std::byte Instruction::getLowerByte() const noexcept {
        return static_cast<std::byte>((mValue & 0x00FF));
    }

    uint16_t Instruction::getNNN() const noexcept {
        return (mValue & 0x0FFF);
    }

    uint8_t Instruction::getNN() const noexcept {
        return gsl::narrow<uint8_t>(mValue & 0x00FF);
    }

    uint8_t Instruction::getN() const noexcept {
        return gsl::narrow<uint8_t>(mValue & 0x000F);
    }

    uint8_t Instruction::getX() const noexcept {
        return gsl::narrow<uint8_t>((mValue & 0x0F00) / 0x0100);
    }

    uint8_t Instruction::getY() const noexcept {
        return gsl::narrow<uint8_t>((mValue & 0x00F0) / 0x0010);
    }

}

