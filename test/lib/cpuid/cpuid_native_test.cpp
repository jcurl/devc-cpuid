#include <gtest/gtest.h>

#include "cpuid/cpuid_native.h"

namespace rjcp::cpuid {

TEST(CpuIdNative, Value)
{
    CpuIdNative cpuid = CpuIdNative{0};
    CpuIdRegister cpuidreg = cpuid.GetCpuId(0x00000000, 0x00000000);

    ASSERT_TRUE(cpuidreg.IsValid());
    ASSERT_NE(cpuidreg.Eax(), 0);
    ASSERT_NE(cpuidreg.Ebx(), 0);
    ASSERT_NE(cpuidreg.Ecx(), 0);
    ASSERT_NE(cpuidreg.Edx(), 0);
}

}
