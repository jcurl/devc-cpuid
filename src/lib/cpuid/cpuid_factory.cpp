#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_device_config.h"
#include "cpuid/cpuid_device.h"
#include "cpuid/cpuid_default_config.h"
#include "cpuid/cpuid_default.h"
#include "cpuid/cpuid_native_config.h"
#include "cpuid/cpuid_native.h"

namespace rjcp::cpuid {

template<>
CpuIdFactory<CpuIdDefaultConfig>::CpuIdFactory() noexcept = default;

template<>
CpuIdFactory<CpuIdDefaultConfig>::CpuIdFactory(CpuIdDefaultConfig& /* config */) noexcept
{
    // There is no configuration.
}

template<>
auto CpuIdFactory<CpuIdDefaultConfig>::create(int cpunum) noexcept -> std::unique_ptr<ICpuId>
{
    return std::make_unique<CpuIdDefault>(cpunum);
}

template<>
CpuIdFactory<CpuIdNativeConfig>::CpuIdFactory() noexcept = default;

template<>
CpuIdFactory<CpuIdNativeConfig>::CpuIdFactory(CpuIdNativeConfig& /* config */) noexcept
{
    // There is no configuration.
}

template<>
auto CpuIdFactory<CpuIdNativeConfig>::create(int cpunum) noexcept -> std::unique_ptr<ICpuId>
{
    return std::make_unique<CpuIdNative>(cpunum);
}

CpuIdFactory<CpuIdDeviceConfig>::CpuIdFactory() noexcept = default;

CpuIdFactory<CpuIdDeviceConfig>::CpuIdFactory(CpuIdDeviceConfig& config) noexcept
    : m_method{config.method}
{ }

auto CpuIdFactory<CpuIdDeviceConfig>::create(int cpunum) noexcept -> std::unique_ptr<ICpuId>
{
    return std::make_unique<CpuIdDevice>(cpunum, m_method);
}

}
