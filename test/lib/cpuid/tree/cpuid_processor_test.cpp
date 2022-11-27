#include <gtest/gtest.h>

#include "cpuid/tree/cpuid_processor.h"

namespace rjcp::cpuid::tree {

TEST(CpuIdProcessor, DefaultConstructor)
{
    CpuIdProcessor processor{};
    ASSERT_EQ(processor.Size(), 0);
    ASSERT_TRUE(processor.IsEmpty());
}

static auto GetReg(std::uint32_t eax, std::uint32_t ecx) -> CpuIdRegister
{
    CpuIdRegister reg{eax, ecx, eax + 0x11111111, ecx + 0x22222222, eax + 0x33333333, ecx + 0x44444444};
    return reg;
}

static void CheckRegister(const CpuIdRegister* reg, std::uint32_t eax, std::uint32_t ecx)
{
    ASSERT_NE(reg, nullptr);
    ASSERT_EQ(reg->InEax(), eax);
    ASSERT_EQ(reg->InEcx(), ecx);
    ASSERT_EQ(reg->Eax(), 0x11111111 + eax);
    ASSERT_EQ(reg->Ebx(), 0x22222222 + ecx);
    ASSERT_EQ(reg->Ecx(), 0x33333333 + eax);
    ASSERT_EQ(reg->Edx(), 0x44444444 + ecx);
}

TEST(CpuIdProcessor, GetLeafNonExistent)
{
    CpuIdProcessor processor{};
    ASSERT_EQ(processor.GetLeaf(0, 0), nullptr);
}

TEST(CpuIdProcessor, AddLeafSingle)
{
    CpuIdRegister reg = GetReg(0, 0);

    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(reg));
    ASSERT_EQ(processor.Size(), 1);
    ASSERT_FALSE(processor.IsEmpty());

    CheckRegister(processor.GetLeaf(0, 0), 0, 0);
    ASSERT_EQ(processor.GetLeaf(0, 1), nullptr);
    ASSERT_EQ(processor.GetLeaf(1, 0), nullptr);
}

TEST(CpuIdProcessor, AddLeafExists)
{
    CpuIdRegister reg = GetReg(0, 0);

    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(reg));
    ASSERT_EQ(processor.Size(), 1);
    ASSERT_FALSE(processor.IsEmpty());

    // Adding the same node twice fails.
    ASSERT_FALSE(processor.AddLeaf(GetReg(0, 0)));
    ASSERT_EQ(processor.Size(), 1);
    ASSERT_FALSE(processor.IsEmpty());
}

TEST(CpuIdProcessor, AddLeafMultiple)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));
    ASSERT_EQ(processor.Size(), 3);
    ASSERT_FALSE(processor.IsEmpty());

    CheckRegister(processor.GetLeaf(0, 0), 0, 0);
    CheckRegister(processor.GetLeaf(1, 0), 1, 0);
    CheckRegister(processor.GetLeaf(13, 1), 13, 1);
    ASSERT_EQ(processor.GetLeaf(0, 1), nullptr);
    ASSERT_EQ(processor.GetLeaf(1, 1), nullptr);
    ASSERT_EQ(processor.GetLeaf(0x80000000, 0), nullptr);
}

TEST(CpuIdProcessor, AddLeafMoveMultiple)
{
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(GetReg(13, 1)));
    ASSERT_TRUE(processor.AddLeaf(GetReg(1, 0)));
    ASSERT_TRUE(processor.AddLeaf(GetReg(0, 0)));
    ASSERT_EQ(processor.Size(), 3);
    ASSERT_FALSE(processor.IsEmpty());

    CheckRegister(processor.GetLeaf(0, 0), 0, 0);
    CheckRegister(processor.GetLeaf(1, 0), 1, 0);
    CheckRegister(processor.GetLeaf(13, 1), 13, 1);
    ASSERT_EQ(processor.GetLeaf(0, 1), nullptr);
    ASSERT_EQ(processor.GetLeaf(1, 1), nullptr);
    ASSERT_EQ(processor.GetLeaf(0x80000000, 0), nullptr);
}

TEST(CpuIdProcessor, IterateEmpty)
{
    CpuIdProcessor processor{};
    auto reg = processor.begin();
    ASSERT_EQ(reg, processor.end());
}

TEST(CpuIdProcessor, Iterate)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    // Insert out of order, and the result should be in order of EAX, ECX.
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));

    auto reg = processor.begin();
    ASSERT_NE(reg, processor.end());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, processor.end());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, processor.end());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, processor.end());
}

TEST(CpuIdProcessor, IterateConstantEmpty)
{
    CpuIdProcessor processor{};
    auto reg = processor.cbegin();
    ASSERT_EQ(reg, processor.cend());
}

TEST(CpuIdProcessor, IterateConstant)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    // Insert out of order, and the result should be in order of EAX, ECX.
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));

    auto reg = processor.cbegin();
    ASSERT_NE(reg, processor.cend());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, processor.cend());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, processor.cend());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, processor.cend());
}

TEST(CpuIdProcessor, CopyConstructorEmpty)
{
    CpuIdProcessor processor{};
    CpuIdProcessor new_processor{processor};
    ASSERT_EQ(processor.Size(), 0);
    ASSERT_EQ(processor.IsEmpty(), true);
}

TEST(CpuIdProcessor, CopyConstructor)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    // Insert out of order, and the result should be in order of EAX, ECX.
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));

    CpuIdProcessor copy_processor{processor};
    ASSERT_EQ(copy_processor.Size(), 3);
    auto reg = copy_processor.cbegin();
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, copy_processor.cend());
}

TEST(CpuIdProcessor, CopyAssignment)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    // Insert out of order, and the result should be in order of EAX, ECX.
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));

    CpuIdProcessor copy_processor = processor;
    ASSERT_EQ(copy_processor.Size(), 3);
    auto reg = copy_processor.cbegin();
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, copy_processor.cend());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, copy_processor.cend());
}

TEST(CpuIdProcessor, MoveConstructor)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    // Insert out of order, and the result should be in order of EAX, ECX.
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(regD1));
    ASSERT_TRUE(processor.AddLeaf(reg10));
    ASSERT_TRUE(processor.AddLeaf(reg00));

    CpuIdProcessor move_processor{std::move(processor)};
    ASSERT_EQ(move_processor.Size(), 3);
    auto reg = move_processor.cbegin();
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, move_processor.cend());
}

TEST(CpuIdProcessor, MoveAssignment)
{
    CpuIdRegister reg00 = GetReg(0, 0);
    CpuIdRegister reg10 = GetReg(1, 0);
    CpuIdRegister regD1 = GetReg(13, 1);

    CpuIdProcessor processor{};
    processor.AddLeaf(regD1);
    processor.AddLeaf(reg10);
    processor.AddLeaf(reg00);

    // These two lines must be separated to have the move assignment operator
    // called.
    CpuIdProcessor move_processor;
    move_processor = CpuIdProcessor{processor};

    ASSERT_EQ(move_processor.Size(), 3);
    auto reg = move_processor.cbegin();
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 0, 0);

    reg++;
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 1, 0);

    reg++;
    ASSERT_NE(reg, move_processor.cend());
    CheckRegister(&reg->second, 13, 1);

    reg++;
    ASSERT_EQ(reg, move_processor.cend());
}

}
