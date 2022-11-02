#include <gtest/gtest.h>

#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_default_config.h"
#include "cpuid/cpuid_device_config.h"
#include "cpuid/cpuid_native_config.h"

namespace rjcp::cpuid {

static auto DumpRegisters(ICpuIdFactory& factory, bool isValid)
{
    // Effectively tests that the factories all implement the same base class
    // for polymorphism.
    auto cpuid = factory.create(0);

    CpuIdRegister reg = cpuid->GetCpuId(0, 0);
    ASSERT_EQ(reg.IsValid(), isValid);

    if (isValid) {
        EXPECT_NE(reg.Eax(), 0);
        EXPECT_NE(reg.Ebx(), 0);
        EXPECT_NE(reg.Ecx(), 0);
        EXPECT_NE(reg.Edx(), 0);
    } else {
        EXPECT_EQ(reg.Eax(), 0);
        EXPECT_EQ(reg.Ebx(), 0);
        EXPECT_EQ(reg.Ecx(), 0);
        EXPECT_EQ(reg.Edx(), 0);
    }
}

TEST(CpuIdFactory, CpuIdDefault)
{
    CpuIdFactory<CpuIdDefaultConfig> factory{};
    DumpRegisters(factory, false);
}

TEST(CpuIdFactory, CpuIdDefaultEmptyConfig)
{
    CpuIdDefaultConfig config{};
    CpuIdFactory<CpuIdDefaultConfig> factory{config};
    DumpRegisters(factory, false);
}

TEST(CpuIdFactory, CpuIdNative)
{
    CpuIdFactory<CpuIdNativeConfig> factory{};
    DumpRegisters(factory, true);
}

TEST(CpuIdFactory, CpuIdNativeEmptyConfig)
{
    CpuIdNativeConfig config{};
    CpuIdFactory<CpuIdNativeConfig> factory{config};
    DumpRegisters(factory, true);
}

TEST(CpuIdFactory, CpuIdDeviceDefault)
{
    CpuIdFactory<CpuIdDeviceConfig> factory{};
    DumpRegisters(factory, true);
}

TEST(CpuIdFactory, CpuIdDeviceSeek)
{
    CpuIdDeviceConfig config{DeviceAccessMethod::seek};
    CpuIdFactory<CpuIdDeviceConfig> factory{config};
    DumpRegisters(factory, true);
}

TEST(CpuIdFactory, CpuIdDevicePread)
{
    CpuIdDeviceConfig config{DeviceAccessMethod::pread};
    CpuIdFactory<CpuIdDeviceConfig> factory{config};
    DumpRegisters(factory, true);
}

}
