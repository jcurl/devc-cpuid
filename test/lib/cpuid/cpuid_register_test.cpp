#include <gtest/gtest.h>

#include "cpuid/cpuid_register.h"

namespace rjcp::cpuid {

TEST(CpuIdRegister, DefaultValue)
{
    CpuIdRegister cpuidreg{};
    ASSERT_FALSE(cpuidreg.IsValid());
    ASSERT_EQ(cpuidreg.InEax(), -1);
    ASSERT_EQ(cpuidreg.InEcx(), -1);
    ASSERT_EQ(cpuidreg.Eax(), 0);
    ASSERT_EQ(cpuidreg.Ebx(), 0);
    ASSERT_EQ(cpuidreg.Ecx(), 0);
    ASSERT_EQ(cpuidreg.Edx(), 0);
}

TEST(CpuIdRegister, Value)
{
    CpuIdRegister cpuidreg{0, 0, 0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    ASSERT_TRUE(cpuidreg.IsValid());
    ASSERT_EQ(cpuidreg.InEax(), 0);
    ASSERT_EQ(cpuidreg.InEcx(), 0);
    ASSERT_EQ(cpuidreg.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg.Edx(), 0x49656E69);
}

TEST(CpuIdRegister, Value2)
{
    CpuIdRegister cpuidreg{1, 2, 0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    ASSERT_TRUE(cpuidreg.IsValid());
    ASSERT_EQ(cpuidreg.InEax(), 1);
    ASSERT_EQ(cpuidreg.InEcx(), 2);
    ASSERT_EQ(cpuidreg.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg.Edx(), 0x49656E69);
}

TEST(CpuIdRegister, CopyConstructor)
{
    CpuIdRegister cpuidreg{0, 0, 0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2{cpuidreg};

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.InEax(), 0);
    ASSERT_EQ(cpuidreg2.InEcx(), 0);
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
}

TEST(CpuIdRegister, CopyAssignment)
{
    CpuIdRegister cpuidreg{0, 0, 0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2 = cpuidreg;

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.InEax(), 0);
    ASSERT_EQ(cpuidreg2.InEcx(), 0);
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
}

TEST(CpuIdRegister, MoveAssignment)
{
    CpuIdRegister cpuidreg{0, 0, 0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2 = std::move(cpuidreg);

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.InEax(), 0);
    ASSERT_EQ(cpuidreg2.InEcx(), 0);
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
}

}
