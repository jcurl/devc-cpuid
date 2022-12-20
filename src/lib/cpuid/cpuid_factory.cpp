#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_device_config.h"
#include "cpuid/cpuid_device.h"
#include "cpuid/cpuid_default_config.h"
#include "cpuid/cpuid_default.h"
#include "cpuid/cpuid_native_config.h"
#include "cpuid/cpuid_native.h"

#include <thread>

namespace rjcp::cpuid
{

namespace
{

template<typename CreationCallback>
class CpuIdFactoryWithCallback : public ICpuIdFactory
{
public:
    CpuIdFactoryWithCallback(const CreationCallback& create_callback)
    : m_create_callback{create_callback}
    { };

    auto create(unsigned int cpunum) noexcept -> std::unique_ptr<ICpuId> override
    {
        return m_create_callback(cpunum);
    }

    auto threads() const -> unsigned int override
    {
        return std::thread::hardware_concurrency();
    }

private:
    CreationCallback m_create_callback;
};

template<typename CreationCallback>
auto MakeCpuIdFactory(const CreationCallback& callback) -> std::unique_ptr<ICpuIdFactory>
{
    return std::make_unique<CpuIdFactoryWithCallback<CreationCallback>>(callback);
}

} // namespace

template<>
auto CreateCpuIdFactory(const CpuIdDefaultConfig &) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([](unsigned int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdDefault>(cpunum); });
}

template<>
auto CreateCpuIdFactory(const CpuIdDeviceConfig &config) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([config](unsigned int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdDevice>(cpunum, config.method); });
}

template<>
auto CreateCpuIdFactory(const CpuIdNativeConfig &) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([](unsigned int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdNative>(cpunum); });
}

}
