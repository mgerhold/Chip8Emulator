#include "Chip8Core/Chip8Opcode.hpp"

#include <regex>

Chip8Opcode::Chip8Opcode(const std::string& opcodeMask)
	: mOpcodeMask(opcodeMask)
{
	mParameterTypes = getParameterTypes();
}

[[nodiscard]] OpcodeParameterTypes Chip8Opcode::getParameterTypes() const {
	if (std::regex_match(mOpcodeMask, std::regex("[0-9A-F]NNN")))
		return OpcodeParameterTypes::NNN;
	if (std::regex_match(mOpcodeMask, std::regex("[0-9A-F]XNN")))
		return OpcodeParameterTypes::XNN;
	if (std::regex_match(mOpcodeMask, std::regex("[0-9A-F]XY[0-9A-F]")))
		return OpcodeParameterTypes::XY;
	if (std::regex_match(mOpcodeMask, std::regex("[0-9A-F]XYN")))
		return OpcodeParameterTypes::XYN;
	if (std::regex_match(mOpcodeMask, std::regex("[0-9A-F]X[0-9A-F][0-9A-F]")))
		return OpcodeParameterTypes::X;
	else
		return OpcodeParameterTypes::None;
}

void Chip8Opcode::setValue(OpcodeValue value) {
	mValue = value;
}

[[nodiscard]] std::optional<uint8_t> Chip8Opcode::getX() const noexcept {
	if (mValue && (
			mParameterTypes == OpcodeParameterTypes::X
			|| mParameterTypes == OpcodeParameterTypes::XNN
			|| mParameterTypes == OpcodeParameterTypes::XY
			|| mParameterTypes == OpcodeParameterTypes::XYN
		)) {
		return static_cast<uint8_t>(mValue.value().getUpperByte() & std::byte{ 0x0F });
	} else {
		return {};
	}
}

[[nodiscard]] std::optional<uint8_t> Chip8Opcode::getY() const noexcept {
	if (mValue && (
		mParameterTypes == OpcodeParameterTypes::XY
		|| mParameterTypes == OpcodeParameterTypes::XYN
	)) {
		return static_cast<uint8_t>(static_cast<uint8_t>((mValue.value().getLowerByte() & std::byte{ 0xF0 })) / 0x0F);
	} else {
		return {};
	}
}

[[nodiscard]] std::optional<uint8_t> Chip8Opcode::getN() const noexcept {
	if (mValue && mParameterTypes == OpcodeParameterTypes::XYN) {
		return static_cast<uint8_t>(mValue.value().getLowerByte() & std::byte{ 0x0F });
	} else {
		return {};
	}
}

[[nodiscard]] std::optional<uint8_t> Chip8Opcode::getNN() const noexcept {
	if (mValue && mParameterTypes == OpcodeParameterTypes::XNN) {
		return static_cast<uint8_t>(mValue.value().getLowerByte());
	} else {
		return {};
	}
}

[[nodiscard]] std::optional<uint16_t> Chip8Opcode::getNNN() const noexcept {
	if (mValue && mParameterTypes == OpcodeParameterTypes::NNN) {		
		return static_cast<uint16_t>(
			static_cast<uint8_t>(mValue.value().getUpperByte() & std::byte{ 0x0F }) * 0x100
			+ static_cast<uint8_t>(mValue.value().getLowerByte())
		);		
	} else {
		return {};
	}
}

OpcodeValue::OpcodeValue(std::byte upperByte, std::byte lowerByte) noexcept
	: mUpperByte(upperByte), mLowerByte(lowerByte)
{}

OpcodeValue::OpcodeValue(uint16_t value) noexcept {
	mLowerByte = static_cast<std::byte>(value & 0xFF);
	mUpperByte = static_cast<std::byte>(value / 0x100);
}

std::byte OpcodeValue::getUpperByte() const noexcept {
	return mUpperByte;
}

std::byte OpcodeValue::getLowerByte() const noexcept {
	return mLowerByte;
}

uint16_t OpcodeValue::getValue() const noexcept {
	return static_cast<uint16_t>(mUpperByte) * 0xFF + static_cast<uint16_t>(mLowerByte);
}
