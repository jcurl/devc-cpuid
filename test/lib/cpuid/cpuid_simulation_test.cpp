#include <gtest/gtest.h>

#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_simulation_config.h"
#include "cpuid/tree/cpuid_tree.h"

#include <iostream>
#include <memory>
#include <utility>

namespace rjcp::cpuid {

TEST(CpuIdFactory, CpuIdSimulation)
{
    tree::CpuIdTree tree{};
    tree::CpuIdProcessor cpu0{};
    cpu0.AddLeaf(CpuIdRegister{0, 0, 1, 2, 3, 4});
    cpu0.AddLeaf(CpuIdRegister{1, 0, 5, 6, 7, 8});
    tree.SetProcessor(0, std::move(cpu0));
    tree::CpuIdProcessor cpu1{};
    cpu1.AddLeaf(CpuIdRegister{0, 0, 2, 3, 4, 5});
    cpu1.AddLeaf(CpuIdRegister{1, 0, 6, 7, 8, 9});
    tree.SetProcessor(1, std::move(cpu1));

    // Create our simulation object based on a tree, and check that we can get
    // the values back. This copies the object above from the stack to a new
    // object in the configuration.
    CpuIdSimulationConfig config{tree};
    ASSERT_EQ(config.GetTree()->Size(), 2);

    auto factory = CreateCpuIdFactory(config);
    ASSERT_EQ(factory->threads(), 2);

    auto cpuid0 = factory->create(0);
    auto cpureg00 = cpuid0->GetCpuId(0, 0);
    EXPECT_EQ(cpureg00.InEax(), 0);
    EXPECT_EQ(cpureg00.InEcx(), 0);
    EXPECT_EQ(cpureg00.Eax(), 1);
    EXPECT_EQ(cpureg00.Ebx(), 2);
    EXPECT_EQ(cpureg00.Ecx(), 3);
    EXPECT_EQ(cpureg00.Edx(), 4);

    auto cpureg01 = cpuid0->GetCpuId(1, 0);
    EXPECT_EQ(cpureg01.InEax(), 1);
    EXPECT_EQ(cpureg01.InEcx(), 0);
    EXPECT_EQ(cpureg01.Eax(), 5);
    EXPECT_EQ(cpureg01.Ebx(), 6);
    EXPECT_EQ(cpureg01.Ecx(), 7);
    EXPECT_EQ(cpureg01.Edx(), 8);

    auto cpuid1 = factory->create(1);
    auto cpureg10 = cpuid1->GetCpuId(0, 0);
    EXPECT_EQ(cpureg10.InEax(), 0);
    EXPECT_EQ(cpureg10.InEcx(), 0);
    EXPECT_EQ(cpureg10.Eax(), 2);
    EXPECT_EQ(cpureg10.Ebx(), 3);
    EXPECT_EQ(cpureg10.Ecx(), 4);
    EXPECT_EQ(cpureg10.Edx(), 5);

    auto cpureg11 = cpuid1->GetCpuId(1, 0);
    EXPECT_EQ(cpureg11.InEax(), 1);
    EXPECT_EQ(cpureg11.InEcx(), 0);
    EXPECT_EQ(cpureg11.Eax(), 6);
    EXPECT_EQ(cpureg11.Ebx(), 7);
    EXPECT_EQ(cpureg11.Ecx(), 8);
    EXPECT_EQ(cpureg11.Edx(), 9);
}

}
