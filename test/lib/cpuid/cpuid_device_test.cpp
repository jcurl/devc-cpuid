#include <gtest/gtest.h>

#include "cpuid/cpuid_device.h"
#include "cpuid/cpuid_native.h"

namespace rjcp::cpuid {

static void CpuIdDeviceTestValue(CpuIdDevice& cpuid)
{
    CpuIdRegister cpuidreg = cpuid.GetCpuId(0x00000000, 0x00000000);

    ASSERT_TRUE(cpuidreg.IsValid()) << "Ensure that the cpuid driver is loaded and has readable permissions";
    ASSERT_NE(cpuidreg.Eax(), 0);
    ASSERT_NE(cpuidreg.Ebx(), 0);
    ASSERT_NE(cpuidreg.Ecx(), 0);
    ASSERT_NE(cpuidreg.Edx(), 0);

    // For sanity, compare against the same which the CPUID instruction reports.
    // The values should match.

    CpuIdNative ncpuid = CpuIdNative{0};
    CpuIdRegister ncpuidreg = ncpuid.GetCpuId(0x00000000, 0x00000000);

    ASSERT_TRUE(ncpuidreg.IsValid());
    EXPECT_EQ(cpuidreg.Eax(), ncpuidreg.Eax());
    EXPECT_EQ(cpuidreg.Ebx(), ncpuidreg.Ebx());
    EXPECT_EQ(cpuidreg.Ecx(), ncpuidreg.Ecx());
    EXPECT_EQ(cpuidreg.Edx(), ncpuidreg.Edx());
}

TEST(CpuIdDevice, Value)
{
    CpuIdDevice cpuid = CpuIdDevice{0};
    CpuIdDeviceTestValue(cpuid);
}

TEST(CpuIdDevice, ValuePread)
{
    CpuIdDevice cpuid = CpuIdDevice{0, DeviceAccessMethod::pread};
    CpuIdDeviceTestValue(cpuid);
}

TEST(CpuIdDevice, ValueLseek)
{
    CpuIdDevice cpuid = CpuIdDevice{0, DeviceAccessMethod::seek};
    CpuIdDeviceTestValue(cpuid);
}

TEST(CpuIdDevice, InvalidCpu)
{
    CpuIdDevice cpuid = CpuIdDevice{256};  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    CpuIdRegister cpuidreg = cpuid.GetCpuId(0x00000000, 0x00000000);

    ASSERT_FALSE(cpuidreg.IsValid());
}

}
