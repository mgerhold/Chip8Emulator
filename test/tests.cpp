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
	class ProgramCounterTest : public ::testing::Test {
	protected:
		::Chip8::Chip8 chip8;
	};

	TEST_F(ProgramCounterTest, ProgramCounterInitializesCorrectly) {
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset);
	}

	TEST_F(ProgramCounterTest, ProgramCounterIncreasesCorrectlyUponStep) {
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x2);
	}
}

namespace {
	class OpcodeTest : public ::testing::Test {
	public:
		OpcodeTest()
			: mWritePosition(::Chip8::Chip8::ProgramOffset)
		{}

	protected:
		::Chip8::Chip8 chip8;

		void writeInstruction(uint16_t instruction/*, uint16_t offset = 0x0*/) {			
			chip8.getMemory().write(mWritePosition++, gsl::narrow<uint8_t>((instruction & 0xFF00) / 0x0100));
			chip8.getMemory().write(mWritePosition++, gsl::narrow<uint8_t>(instruction & 0x00FF));
		}

	private:
		uint16_t mWritePosition;
	};

	TEST_F(OpcodeTest, ReturnFromSubroutine) { // 0x00EE
		chip8.stackPush(0x123);
		writeInstruction(0x00EE); // instruction = return from subroutine
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), 0x0123);
	}

	TEST_F(OpcodeTest, JumpToSubroutine) { // 0x2NNN
		writeInstruction(0x2123); // instruction = jump to subroutine at 0x123
		chip8.step();
		ASSERT_EQ(chip8.stackPop(), chip8.ProgramOffset + 0x2);
		ASSERT_EQ(chip8.getProgramCounter(), 0x123);
	}

	TEST_F(OpcodeTest, JumpToAddress) { // 0x1NNN
		writeInstruction(0x1ABC); // instruction = jump to address 0xABC
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), 0x0ABC);
	}

	TEST_F(OpcodeTest, SkipIfEqual_Skip) { // 0x3XNN
		chip8.setRegister(0xA, 0xFF);
		writeInstruction(0x3AFF); // instruction = skip next instruction if VA is 0xFF
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x4);
	}

	TEST_F(OpcodeTest, SkipIfEqual_NoSkip) { // 0x3XNN
		chip8.setRegister(0xA, 0xCC);
		writeInstruction(0x3AFF); // instruction = skip next instruction if VA is 0xFF
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x2);
	}

	TEST_F(OpcodeTest, SkipIfNotEqual_Skip) { // 0x4XNN
		chip8.setRegister(0xA, 0xBB);
		writeInstruction(0x4AFF); // instruction = skip next instruction if VA is not 0xFF
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x4);
	}

	TEST_F(OpcodeTest, SkipIfNotEqual_NoSkip) { // 0x4XNN
		chip8.setRegister(0xA, 0xFF);
		writeInstruction(0x4AFF); // instruction = skip next instruction if VA is not 0xFF
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x2);
	}

	TEST_F(OpcodeTest, SkipIfRegistersAreEqual_Skip) { // 0x5XY0
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0x12);
		writeInstruction(0x5AB0); // instruction = skip next instruction if VA is equal to VB
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x4);
	}

	TEST_F(OpcodeTest, SkipIfRegistersAreEqual_NoSkip) { // 0x5XY0
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0x13);
		writeInstruction(0x5AB0); // instruction = skip next instruction if VA is equal to VB
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x2);
	}

	TEST_F(OpcodeTest, SetRegister_ReadRegister) {  // 0x6XNN
		writeInstruction(0x63AA); // instruction = write AA into register V3
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0x3), 0xAA);
	}

	TEST_F(OpcodeTest, AddConstantToRegister_NoOverflow) { // 0x7XNN
		chip8.setRegister(0xA, 0x12);
		writeInstruction(0x7A25); // instruction = add 0x25 to VA
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0x12 + 0x25);
	}

	TEST_F(OpcodeTest, AddConstantToRegister_Overflow) { // 0x7XNN
		chip8.setRegister(0xA, 0xFF);
		writeInstruction(0x7A05); // instruction = add 0x05 to VA
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0x05 - 0x01);
	}

	TEST_F(OpcodeTest, SetRegisterToValueOfAnotherRegister) { // 0x8XY0
		chip8.setRegister(0xC, 0x12);
		writeInstruction(0x82C0); // instruction = set V2 to the value of VC (which is 0x12)
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0x2), 0x12);
	}

	TEST_F(OpcodeTest, BitwiseOrTwoRegisters) { // 0x8XY1
		chip8.setRegister(0xC, 0x12);
		chip8.setRegister(0x3, 0xA4);
		writeInstruction(0x8C31); // instruction = set VC to the bitwise OR of VC and V3
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x12 | 0xA4);
	}

	TEST_F(OpcodeTest, BitwiseAndTwoRegisters) { // 0x8XY2
		chip8.setRegister(0xC, 0x12);
		chip8.setRegister(0x3, 0xA4);
		writeInstruction(0x8C32); // instruction = set VC to the bitwise AND of VC and V3
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x12 & 0xA4);
	}

	TEST_F(OpcodeTest, BitwiseXorTwoRegisters) { // 0x8XY3
		chip8.setRegister(0xC, 0x12);
		chip8.setRegister(0x3, 0xA4);
		writeInstruction(0x8C33); // instruction = set VC to the bitwise XOR of VC and V3
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x12 ^ 0xA4);
	}

	TEST_F(OpcodeTest, AddRegistersWithCarryFlag_Carry) { // 0x8XY4
		chip8.setRegister(0xC, 0xFF);
		chip8.setRegister(0x3, 0x24);
		writeInstruction(0x8C34); // instruction = add V3 (0x24) to VC (0xFF)
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x24 - 0x1);
		ASSERT_EQ(chip8.getRegister(0xF), 0x1); // carry flag should be set now
	}

	TEST_F(OpcodeTest, AddRegistersWithCarryFlag_NoCarry) { // 0x8XY4
		chip8.setRegister(0xC, 0x05);
		chip8.setRegister(0x3, 0x24);
		writeInstruction(0x8C34); // instruction = add V3 (0x24) to VC (0x05)
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x05 + 0x24);
		ASSERT_EQ(chip8.getRegister(0xF), 0x0); // carry flag should not be set now
	}

	TEST_F(OpcodeTest, SubtractRegisters_Borrow) { // 0x8XY5
		chip8.setRegister(0xC, 0x00);
		chip8.setRegister(0x3, 0x24);
		writeInstruction(0x8C35); // instruction = subtract V3 (0x24) from VC (0x05)
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0xFF - 0x23);
		ASSERT_EQ(chip8.getRegister(0xF), 0x0); // borrow
	}

	TEST_F(OpcodeTest, SubtractRegisters_NoBorrow) { // 0x8XY5
		chip8.setRegister(0xC, 0x24);
		chip8.setRegister(0x3, 0x05);
		writeInstruction(0x8C35); // instruction = subtract V3 (0x05) from VC (0x24)
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xC), 0x24 - 0x05);
		ASSERT_EQ(chip8.getRegister(0xF), 0x1); // no borrow
	}

	TEST_F(OpcodeTest, BitShiftRight_OriginalBehavior) { // 0x8XY6
		chip8.setCompatibilityMode(CompatibilityMode::OriginalChip8);
		chip8.setRegister(0xB, 0b1001);
		writeInstruction(0x8AB6); // instruction = shift VB to the right, save in VA. Save the "out-shifted" bit in VF
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0b0100);
		ASSERT_EQ(chip8.getRegister(0xB), 0b1001); // no change
		ASSERT_EQ(chip8.getRegister(0xF), 0b0001);
	}

	TEST_F(OpcodeTest, BitShiftRight_SuperChipBehavior) { // 0x8XY6
		chip8.setCompatibilityMode(CompatibilityMode::SuperChip);
		chip8.setRegister(0xB, 0b1001);
		writeInstruction(0x8BC6); // instruction = shift VB to the right. Save the "out-shifted" bit in VF. VC is ignored.
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xB), 0b0100);
		ASSERT_EQ(chip8.getRegister(0xC), 0b0000); // no change
		ASSERT_EQ(chip8.getRegister(0xF), 0b0001);
	}

	TEST_F(OpcodeTest, SubstractRegistersDifferentOrder_NoBorrow) { // 0x8XY7
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0xA2);
		writeInstruction(0x8AB7); // instruction = calculate VB - VA, store in VA
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0xA2 - 0x12);
		ASSERT_EQ(chip8.getRegister(0xF), 0x1); // no borrow
	}

	TEST_F(OpcodeTest, SubstractRegistersDifferentOrder_Borrow) { // 0x8XY7
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0x00);
		writeInstruction(0x8AB7); // instruction = calculate VB - VA, store in VA
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0xFF - 0x11);
		ASSERT_EQ(chip8.getRegister(0xF), 0x0); // borrow
	}

	TEST_F(OpcodeTest, BitShiftLeft_OriginalBehavior) { // 0x8XYE
		chip8.setCompatibilityMode(CompatibilityMode::OriginalChip8);
		chip8.setRegister(0xB, 0b1010'1001);
		writeInstruction(0x8ABE); // instruction = shift VB to the left, save in VA. Save the "out-shifted" bit in VF
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xA), 0b0101'0010);
		ASSERT_EQ(chip8.getRegister(0xB), 0b1010'1001); // no change
		ASSERT_EQ(chip8.getRegister(0xF), 0b0000'0001);
	}

	TEST_F(OpcodeTest, BitShiftLeft_SuperChipBehavior) { // 0x8XYE
		chip8.setCompatibilityMode(CompatibilityMode::SuperChip);
		chip8.setRegister(0xB, 0b1010'1001);
		writeInstruction(0x8BCE); // instruction = shift VB to the left. Save the "out-shifted" bit in VF. VC is ignored.
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0xB), 0b0101'0010);
		ASSERT_EQ(chip8.getRegister(0xC), 0b0000'0000); // no change
		ASSERT_EQ(chip8.getRegister(0xF), 0b0000'0001);
	}

	TEST_F(OpcodeTest, SkipIfRegistersAreNotEqual_Skip) { // 0x9XY0
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0x13);
		writeInstruction(0x9AB0); // instruction = skip if VA is not equal to VB
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x4);
	}

	TEST_F(OpcodeTest, SkipIfRegistersAreNotEqual_NoSkip) { // 0x9XY0
		chip8.setRegister(0xA, 0x12);
		chip8.setRegister(0xB, 0x12);
		writeInstruction(0x9AB0); // instruction = skip if VA is not equal to VB
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), chip8.ProgramOffset + 0x2);
	}

	TEST_F(OpcodeTest, SetAddressPointer) { // 0xANNN
		writeInstruction(0xA123); // instruction = set address pointer I to 0x123
		chip8.step();
		ASSERT_EQ(chip8.getAddressPointer(), 0x0123);
	}

	TEST_F(OpcodeTest, JumpToV0PlusOffset) { // 0xBNNN
		chip8.setRegister(0x0, 0x025);
		writeInstruction(0xB123); // jump to V0 + 0x123
		chip8.step();
		ASSERT_EQ(chip8.getProgramCounter(), 0x25 + 0x123);
	}

	TEST_F(OpcodeTest, GenerateRandomNumberWithMax) { // 0xCXNN
		bool greaterThanZeroFound = false;
		for (uint8_t i = 0x0; i < 0xFF; i++) {
			writeInstruction(0xCA00 | i);
			chip8.step();
			ASSERT_LE(chip8.getRegister(0xA), i);
			if (chip8.getRegister(0xA) > 0x0)
				greaterThanZeroFound = true;
		}
		ASSERT_TRUE(greaterThanZeroFound);
	}

	TEST_F(OpcodeTest, LoadDelayTimerIntroRegister) { // FX07
		chip8.setRegister(0xA, 0x24);
		writeInstruction(0xFA15); // set delay timer to VA (0x24)
		writeInstruction(0xFB07); // load value of delay timer into VB
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getDelayTimer(), 0x24);
	}

	TEST_F(OpcodeTest, SetDelayTimer) { // 0xFX15
		chip8.setRegister(0xA, 0x025);
		writeInstruction(0xFA15); // set delay timer to VA (0x025)
		chip8.step();
		ASSERT_EQ(chip8.getDelayTimer(), 0x25);
	}

	TEST_F(OpcodeTest, AddRegisterToAddressPointer) { // 0xFX1E
		chip8.setRegister(0xA, 0x025);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xFA1E); // add VA to the address pointer
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getAddressPointer(), 0x025 + 0x123);
	}

	TEST_F(OpcodeTest, DecimalRepresentation) { // 0xFX33
		chip8.setRegister(0xA, 127);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xFA33); // store decimal representation of VA at address pointer
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getMemory().read(chip8.getAddressPointer() + 0x0), 1);
		ASSERT_EQ(chip8.getMemory().read(chip8.getAddressPointer() + 0x1), 2);
		ASSERT_EQ(chip8.getMemory().read(chip8.getAddressPointer() + 0x2), 7);
	}

	TEST_F(OpcodeTest, DumpRegistersToMemory_OriginalBehavior) { // 0xFX55
		chip8.setCompatibilityMode(CompatibilityMode::OriginalChip8);
		chip8.setRegister(0x0, 0x42);
		chip8.setRegister(0x1, 0xAB);
		chip8.setRegister(0x2, 0xFF);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xF255); // dump V0 to V2 into memory at current address pointer position
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getMemory().read(0x123), 0x42);
		ASSERT_EQ(chip8.getMemory().read(0x124), 0xAB);
		ASSERT_EQ(chip8.getMemory().read(0x125), 0xFF);
		ASSERT_EQ(chip8.getAddressPointer(), 0x123 + 3); // address pointer also increased
	}

	TEST_F(OpcodeTest, DumpRegistersToMemory_SuperChipBehavior) { // 0xFX55
		chip8.setCompatibilityMode(CompatibilityMode::SuperChip);
		chip8.setRegister(0x0, 0x42);
		chip8.setRegister(0x1, 0xAB);
		chip8.setRegister(0x2, 0xFF);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xF255); // dump V0 to V2 into memory at current address pointer position
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getMemory().read(0x123), 0x42);
		ASSERT_EQ(chip8.getMemory().read(0x124), 0xAB);
		ASSERT_EQ(chip8.getMemory().read(0x125), 0xFF);
		ASSERT_EQ(chip8.getAddressPointer(), 0x123); // address pointer unchanged
	}

	TEST_F(OpcodeTest, LoadRegistersFromMemory_OriginalBehavior) { // FX65
		chip8.setCompatibilityMode(CompatibilityMode::OriginalChip8);
		chip8.getMemory().write(0x123, 0x17);
		chip8.getMemory().write(0x124, 0xAB);
		chip8.getMemory().write(0x125, 0x42);
		chip8.getMemory().write(0x126, 0x00);
		chip8.getMemory().write(0x127, 0xFF);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xF465); // load V0 to V4 from memory
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0x0), 0x17);
		ASSERT_EQ(chip8.getRegister(0x1), 0xAB);
		ASSERT_EQ(chip8.getRegister(0x2), 0x42);
		ASSERT_EQ(chip8.getRegister(0x3), 0x00);
		ASSERT_EQ(chip8.getRegister(0x4), 0xFF);
		ASSERT_EQ(chip8.getAddressPointer(), 0x123 + 5); // address pointer also increased
	}

	TEST_F(OpcodeTest, LoadRegistersFromMemory_SuperChipBehavior) { // FX65
		chip8.setCompatibilityMode(CompatibilityMode::SuperChip);
		chip8.getMemory().write(0x123, 0x17);
		chip8.getMemory().write(0x124, 0xAB);
		chip8.getMemory().write(0x125, 0x42);
		chip8.getMemory().write(0x126, 0x00);
		chip8.getMemory().write(0x127, 0xFF);
		writeInstruction(0xA123); // set address pointer to 0x123
		writeInstruction(0xF465); // load V0 to V4 from memory
		chip8.step();
		chip8.step();
		ASSERT_EQ(chip8.getRegister(0x0), 0x17);
		ASSERT_EQ(chip8.getRegister(0x1), 0xAB);
		ASSERT_EQ(chip8.getRegister(0x2), 0x42);
		ASSERT_EQ(chip8.getRegister(0x3), 0x00);
		ASSERT_EQ(chip8.getRegister(0x4), 0xFF);
		ASSERT_EQ(chip8.getAddressPointer(), 0x123); // address pointer unchanged
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}