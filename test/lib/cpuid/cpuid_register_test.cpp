#include <gtest/gtest.h>

#include "cpuid/cpuid_register.h"

namespace rjcp::cpuid {

TEST(CpuIdRegister, DefaultValue)
{
    CpuIdRegister cpuidreg{};
    ASSERT_FALSE(cpuidreg.IsValid());
    ASSERT_EQ(cpuidreg.Eax(), 0);
    ASSERT_EQ(cpuidreg.Ebx(), 0);
    ASSERT_EQ(cpuidreg.Ecx(), 0);
    ASSERT_EQ(cpuidreg.Edx(), 0);
}

TEST(CpuIdRegister, Value)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    CpuIdRegister cpuidreg{0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    ASSERT_TRUE(cpuidreg.IsValid());
    ASSERT_EQ(cpuidreg.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg.Edx(), 0x49656E69);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(CpuIdRegister, CopyConstructor)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    CpuIdRegister cpuidreg{0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2{cpuidreg};

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(CpuIdRegister, CopyAssignment)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    CpuIdRegister cpuidreg{0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2 = cpuidreg;

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

TEST(CpuIdRegister, MoveAssignment)
{
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    CpuIdRegister cpuidreg{0x00000014, 0x756E6547, 0x6C65746E, 0x49656E69};
    CpuIdRegister cpuidreg2 = std::move(cpuidreg);

    ASSERT_TRUE(cpuidreg2.IsValid());
    ASSERT_EQ(cpuidreg2.Eax(), 0x00000014);
    ASSERT_EQ(cpuidreg2.Ebx(), 0x756E6547);
    ASSERT_EQ(cpuidreg2.Ecx(), 0x6C65746E);
    ASSERT_EQ(cpuidreg2.Edx(), 0x49656E69);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}

}
