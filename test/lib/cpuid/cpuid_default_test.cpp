#include <gtest/gtest.h>

#include "cpuid/cpuid_default.h"

namespace rjcp::cpuid {

TEST(CpuIdDefault, DefaultValue)
{
    CpuIdDefault cpuid = CpuIdDefault{0};
    CpuIdRegister cpuidreg = cpuid.GetCpuId(0x00000000, 0x00000000);

    ASSERT_FALSE(cpuidreg.IsValid());   // The default class doesn't have valid data.
    ASSERT_EQ(cpuidreg.Eax(), 0);
    ASSERT_EQ(cpuidreg.Ebx(), 0);
    ASSERT_EQ(cpuidreg.Ecx(), 0);
    ASSERT_EQ(cpuidreg.Edx(), 0);
}

}
