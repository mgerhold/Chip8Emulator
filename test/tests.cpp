#ifdef _MSC_VER
#pragma warning(disable: 26812 26495)
#endif

#include <gtest/gtest.h>

#ifdef _MSC_VER
#pragma warning(default: 26812 26495)
#endif

#include <random>
#include <gsl/gsl>

#include <Chip8Core/Chip8.hpp>
#include <Chip8Core/Memory.hpp>
#include <Chip8Core/Instruction.hpp>

using namespace Chip8;

namespace {
	class MemoryTests : public ::testing::Test {
	protected:
		using UnderlyingType = ::Chip8::Chip8::MemoryUnderlyingType;
		Chip8Memory<UnderlyingType> memory;
	};

	TEST_F(MemoryTests, IsZeroInitialized) {
		for (size_t i = 0; i < 0x1000 /* = 4096 */; i++) {
			ASSERT_TRUE(memory.read(0x0) == 0);
		}
	}

	TEST_F(MemoryTests, OutOfBoundsAccessThrowsException) {
		ASSERT_THROW(static_cast<void>(memory.read(0x1000)), std::out_of_range);
		ASSERT_THROW(memory.write(0x1000, 0), std::out_of_range);
	}

	TEST_F(MemoryTests, WriteValue_ReadValue) {
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(0x00, 0xFF /* = 0x1000 - 0x0001 */);
		for (uint16_t i = 0x0; i < 0x1000; i++) {
			auto randomValue = distribution(generator);
			memory.write(i, static_cast<MemoryTests::UnderlyingType>(randomValue));
			ASSERT_EQ(static_cast<MemoryTests::UnderlyingType>(randomValue), memory.read(i));
		}
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

namespace {
	class OpcodeTest : public ::testing::Test {
	protected:
		::Chip8::Chip8 chip8;

		void writeInstruction(uint16_t instruction, uint16_t address = ::Chip8::Chip8::ProgramOffset) {
			chip8.getMemory().write(address, gsl::narrow<uint8_t>((instruction & 0xFF00) / 0x0100));
			chip8.getMemory().write(address + 0x1, gsl::narrow<uint8_t>(instruction & 0x00FF));
		}
	};

	TEST_F(OpcodeTest, SetRegister_ReadRegister) {
		writeInstruction(0x63AA); // instruction = write AA into register V3
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0x3), 0xAA);
	}

}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}