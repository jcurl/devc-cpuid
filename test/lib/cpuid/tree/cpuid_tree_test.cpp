#include <gtest/gtest.h>

#include "cpuid/tree/cpuid_tree.h"

namespace rjcp::cpuid::tree {

TEST(CpuIdTree, DefaultConstructor)
{
    CpuIdTree tree{};
    ASSERT_EQ(tree.Size(), 0);
    ASSERT_TRUE(tree.IsEmpty());
}

TEST(CpuIdTree, GetProcessorNonExistent)
{
    CpuIdTree tree{};
    ASSERT_EQ(tree.GetProcessor(0), nullptr);
}

TEST(CpuIdTree, GetProcessorNegative)
{
    CpuIdTree tree{};
    ASSERT_EQ(tree.GetProcessor(-1), nullptr);
}

static auto GetReg(std::uint32_t eax, std::uint32_t ecx) -> CpuIdRegister
{
    CpuIdRegister reg{eax, ecx, eax + 0x55555555, ecx + 0x66666666, eax + 0x77777777, ecx + 0x22222222};
    return reg;
}

TEST(CpuIdTree, SetProcessorCopyConstructor)
{
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(GetReg(0, 0)));

    CpuIdTree tree{};
    ASSERT_TRUE(tree.SetProcessor(0, processor));
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_FALSE(tree.IsEmpty());
    ASSERT_TRUE(tree.SetProcessor(1, CpuIdProcessor{}));
    ASSERT_EQ(tree.Size(), 2);
    ASSERT_FALSE(tree.IsEmpty());

    const CpuIdProcessor* value = tree.GetProcessor(0);
    ASSERT_NE(value, nullptr);
    ASSERT_NE(value->GetLeaf(0, 0), nullptr);

    CpuIdTree tree_copy{tree};
    ASSERT_EQ(tree_copy.Size(), 2);
    ASSERT_FALSE(tree_copy.IsEmpty());
    const CpuIdProcessor* value_copy = tree_copy.GetProcessor(0);
    ASSERT_NE(value_copy, nullptr);
    ASSERT_NE(value_copy->GetLeaf(0, 0), nullptr);
    const CpuIdProcessor* value_copy2 = tree_copy.GetProcessor(1);
    ASSERT_NE(value_copy2, nullptr);
    ASSERT_EQ(value_copy2->Size(), 0);
}

TEST(CpuIdTree, SetProcessorCopyAssignment)
{
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(GetReg(0, 0)));

    CpuIdTree tree{};
    ASSERT_TRUE(tree.SetProcessor(0, processor));
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_FALSE(tree.IsEmpty());

    const CpuIdProcessor* value = tree.GetProcessor(0);
    ASSERT_NE(value, nullptr);
    ASSERT_NE(value->GetLeaf(0, 0), nullptr);

    CpuIdTree tree_copy = tree;
    ASSERT_EQ(tree_copy.Size(), 1);
    ASSERT_FALSE(tree_copy.IsEmpty());
    const CpuIdProcessor* value_copy = tree_copy.GetProcessor(0);
    ASSERT_NE(value_copy, nullptr);
    ASSERT_NE(value_copy->GetLeaf(0, 0), nullptr);
}

TEST(CpuIdTree, SetProcessorMoveConstructor)
{
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(GetReg(0, 0)));

    CpuIdTree tree{};
    ASSERT_TRUE(tree.SetProcessor(0, processor));
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_FALSE(tree.IsEmpty());

    const CpuIdProcessor* value = tree.GetProcessor(0);
    ASSERT_NE(value, nullptr);
    ASSERT_NE(value->GetLeaf(0, 0), nullptr);

    CpuIdTree tree_move{std::move(tree)};
    ASSERT_EQ(tree_move.Size(), 1);
    ASSERT_FALSE(tree_move.IsEmpty());
    const CpuIdProcessor* value_move = tree_move.GetProcessor(0);
    ASSERT_NE(value_move, nullptr);
    ASSERT_NE(value_move->GetLeaf(0, 0), nullptr);
}

TEST(CpuIdTree, SetProcessorMoveAssignment)
{
    CpuIdProcessor processor{};
    ASSERT_TRUE(processor.AddLeaf(GetReg(0, 0)));

    CpuIdTree tree{};
    ASSERT_TRUE(tree.SetProcessor(0, processor));
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_FALSE(tree.IsEmpty());

    const CpuIdProcessor* value = tree.GetProcessor(0);
    ASSERT_NE(value, nullptr);
    ASSERT_NE(value->GetLeaf(0, 0), nullptr);

    CpuIdTree tree_move;
    tree_move = std::move(tree);
    ASSERT_EQ(tree_move.Size(), 1);
    ASSERT_FALSE(tree_move.IsEmpty());
    const CpuIdProcessor* value_move = tree_move.GetProcessor(0);
    ASSERT_NE(value_move, nullptr);
    ASSERT_NE(value_move->GetLeaf(0, 0), nullptr);
}

TEST(CpuIdTree, Iterate)
{
    CpuIdTree tree{};

    CpuIdProcessor processor0{};
    ASSERT_TRUE(processor0.AddLeaf(GetReg(0, 0)));
    ASSERT_TRUE(processor0.AddLeaf(GetReg(1, 0)));

    CpuIdProcessor processor1{};
    ASSERT_TRUE(processor1.AddLeaf(GetReg(0, 1)));

    ASSERT_TRUE(tree.SetProcessor(0, processor0));
    ASSERT_TRUE(tree.SetProcessor(1, processor1));

    auto processor = tree.begin();
    ASSERT_NE(processor, tree.end());
    ASSERT_EQ(processor->second.Size(), 2);

    processor++;
    ASSERT_NE(processor, tree.end());
    ASSERT_EQ(processor->second.Size(), 1);

    processor++;
    ASSERT_EQ(processor, tree.end());
}

TEST(CpuIdTree, IterateConstant)
{
    CpuIdTree tree{};

    CpuIdProcessor processor0{};
    ASSERT_TRUE(processor0.AddLeaf(GetReg(0, 0)));
    ASSERT_TRUE(processor0.AddLeaf(GetReg(1, 0)));

    CpuIdProcessor processor1{};
    ASSERT_TRUE(processor1.AddLeaf(GetReg(0, 1)));

    ASSERT_TRUE(tree.SetProcessor(0, processor0));
    ASSERT_TRUE(tree.SetProcessor(1, processor1));

    auto processor = tree.cbegin();
    ASSERT_NE(processor, tree.cend());
    ASSERT_EQ(processor->second.Size(), 2);

    processor++;
    ASSERT_NE(processor, tree.cend());
    ASSERT_EQ(processor->second.Size(), 1);

    processor++;
    ASSERT_EQ(processor, tree.cend());
}

}
