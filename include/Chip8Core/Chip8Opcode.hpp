#pragma once

#include <string>
#include <optional>
#include <variant>
#include <cstdint>
#include <cstddef>

/*enum class OpcodeParameters {
	None,
	AddressNNN,
	XRegisterAnd8BitConstantNN,
	XRegisterAndYRegister,
	XRegisterAndYRegisterAnd4BitConstantN,
};*/

class OpcodeValue {
public:
	explicit OpcodeValue(std::byte upperByte, std::byte lowerByte) noexcept;
	explicit OpcodeValue(uint16_t value) noexcept;

	std::byte getUpperByte() const noexcept;
	std::byte getLowerByte() const noexcept;
	uint16_t getValue() const noexcept;
private:
	std::byte mUpperByte;
	std::byte mLowerByte;
};

enum class OpcodeParameterTypes {
	None,
	NNN,
	XNN,
	XY,
	XYN,
	X,
};

using NNN_t = struct { uint16_t nnn; };
using XNN_t = struct { uint8_t x; uint8_t nn; };
using XY_t = struct { uint8_t x; uint8_t y; };
using XYN_t = struct { uint8_t x; uint8_t y; uint8_t n; };
using X_t = struct { uint8_t x; };
using OpcodeParametersVariant = std::variant<NNN_t, XNN_t, XY_t, XYN_t, X_t>;
using OpcodeParameters = std::optional<OpcodeParametersVariant>;

class Chip8Opcode {
public:
	Chip8Opcode(const std::string& opcodeMask);
	OpcodeParameterTypes getParameterTypes() const;
	void setValue(OpcodeValue value);
	std::optional<uint8_t> getX() const noexcept;
	std::optional<uint8_t> getY() const noexcept;
	std::optional<uint8_t> getN() const noexcept;
	std::optional<uint8_t> getNN() const noexcept;
	std::optional<uint16_t> getNNN() const noexcept;

private:
	std::string mOpcodeMask;
	OpcodeParameterTypes mParameterTypes;
	std::optional<OpcodeValue> mValue;
};