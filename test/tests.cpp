#ifdef _MSC_VER
#pragma warning(disable: 26812 26495)
#endif

#include <gtest/gtest.h>

#ifdef _MSC_VER
#pragma warning(default: 26812 26495)
#endif

#include <random>

#include <Chip8Core/Chip8Memory.hpp>
#include <Chip8Core/Chip8Opcode.hpp>

// Chip8MemoryTests
TEST(Chip8MemoryTests, ZeroInitialization) {
	Chip8Memory c8m;
	for (size_t i = 0; i < 0x1000 /* = 4096 */; i++) {
		ASSERT_TRUE(c8m.read(0x0) == std::byte{ 0 });
	}
}

TEST(Chip8MemoryTests, OutOfBoundsCheck) {
	Chip8Memory c8m;
	ASSERT_THROW(c8m.read(0x1000), std::out_of_range);
	ASSERT_THROW(c8m.write(0x1000, std::byte{ 0 }), std::out_of_range);
}

TEST(Chip8MemoryTests, ReadBackWrittenValue) {
	Chip8Memory c8m;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0x00, 0xFF /* = 0x1000 - 0x0001 */);
	for (size_t i = 0x0; i < 0x1000; i++) {
		auto randomValue = distribution(generator);
		c8m.write(i, std::byte{ randomValue });
		ASSERT_EQ(randomValue, static_cast<unsigned char>(c8m.read(i)));
	}
}

// Chip8OpcodeTests
TEST(Chip8OpcodeTests, DetermineCorrectParameterTypes) {
	Chip8Opcode opcode("0NNN");	ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::NNN);
	opcode = Chip8Opcode("00E0"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::None);
	opcode = Chip8Opcode("00EE"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::None);
	opcode = Chip8Opcode("1NNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::NNN);
	opcode = Chip8Opcode("2NNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::NNN);
	opcode = Chip8Opcode("3XNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XNN);
	opcode = Chip8Opcode("4XNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XNN);
	opcode = Chip8Opcode("5XY0"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("6XNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XNN);
	opcode = Chip8Opcode("7XNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XNN);
	opcode = Chip8Opcode("8XY0"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY1"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY2"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY3"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY4"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY5"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY6"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XY7"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("8XYE"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("9XY0"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XY);
	opcode = Chip8Opcode("ANNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::NNN);
	opcode = Chip8Opcode("BNNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::NNN);
	opcode = Chip8Opcode("CXNN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XNN);
	opcode = Chip8Opcode("DXYN"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::XYN);
	opcode = Chip8Opcode("EX9E"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("EXA1"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX07"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX0A"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX15"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX18"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX1E"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX29"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX33"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX55"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
	opcode = Chip8Opcode("FX65"); ASSERT_EQ(opcode.getParameterTypes(), OpcodeParameterTypes::X);
}


TEST(Chip8OpcodeTests, GetParameters) {
	Chip8Opcode opcode("3NNN");
	opcode.setValue(OpcodeValue(0x3123));
	ASSERT_EQ(opcode.getNNN().value(), 0x0123);

	opcode = Chip8Opcode("4XNN");
	opcode.setValue(OpcodeValue(0x4B34));
	ASSERT_EQ(opcode.getX().value(), 0xB);
	ASSERT_EQ(opcode.getNN().value(), 0x34);

	opcode = Chip8Opcode("8XY2");
	opcode.setValue(OpcodeValue(0x8AB2));
	ASSERT_EQ(opcode.getX().value(), 0xA);
	ASSERT_EQ(opcode.getY().value(), 0xB);

	opcode = Chip8Opcode("DXYN");
	opcode.setValue(OpcodeValue(0xDABC));
	ASSERT_EQ(opcode.getX().value(), 0xA);
	ASSERT_EQ(opcode.getY().value(), 0xB);
	ASSERT_EQ(opcode.getN().value(), 0xC);

	opcode = Chip8Opcode("FX33");
	opcode.setValue(OpcodeValue(0xFA33));
	ASSERT_EQ(opcode.getX().value(), 0xA);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}