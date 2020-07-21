#ifdef _MSC_VER
#pragma warning(disable: 26812 26495)
#endif

#include <gtest/gtest.h>

#ifdef _MSC_VER
#pragma warning(default: 26812 26495)
#endif

#include <random>

#include <Chip8Core/Memory.hpp>
#include <Chip8Core/Instruction.hpp>

using namespace Chip8;

// Chip8MemoryTests
TEST(Chip8MemoryTests, ZeroInitialization) {
	Chip8Memory<char> c8m;
	for (size_t i = 0; i < 0x1000 /* = 4096 */; i++) {
		ASSERT_TRUE(c8m.read(0x0) == 0);
	}
}

TEST(Chip8MemoryTests, OutOfBoundsCheck) {
	Chip8Memory<char> c8m;
	ASSERT_THROW(static_cast<void>(c8m.read(0x1000)), std::out_of_range);
	ASSERT_THROW(c8m.write(0x1000, 0), std::out_of_range);
}

TEST(Chip8MemoryTests, ReadBackWrittenValue) {
	Chip8Memory<char> c8m;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0x00, 0xFF /* = 0x1000 - 0x0001 */);
	for (uint16_t i = 0x0; i < 0x1000; i++) {
		auto randomValue = distribution(generator);
		c8m.write(i, static_cast<char>(randomValue));
		ASSERT_EQ(static_cast<char>(randomValue), c8m.read(i));
	}
}

// Chip8Instruction tests
TEST(Chip8InstructionTests, GetParameters) {
	Instruction instruction(0x2ABC);
	ASSERT_EQ(instruction.getNNN(), 0xABC);

	instruction = Instruction(0x63BC);
	ASSERT_EQ(instruction.getX(), 0x3);
	ASSERT_EQ(instruction.getNN(), 0xBC);

	instruction = Instruction(0x8AB2);
	ASSERT_EQ(instruction.getX(), 0xA);
	ASSERT_EQ(instruction.getY(), 0xB);

	instruction = Instruction(0xDAB5);
	ASSERT_EQ(instruction.getX(), 0xA);
	ASSERT_EQ(instruction.getY(), 0xB);
	ASSERT_EQ(instruction.getN(), 0x5);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}