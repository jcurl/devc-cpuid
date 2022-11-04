#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_device_config.h"
#include "cpuid/cpuid_device.h"
#include "cpuid/cpuid_default_config.h"
#include "cpuid/cpuid_default.h"
#include "cpuid/cpuid_native_config.h"
#include "cpuid/cpuid_native.h"

namespace rjcp::cpuid
{

namespace
{

template <typename CreationCallback>
class CpuIdFactoryWithCallback : public ICpuIdFactory
{
public:
    CpuIdFactoryWithCallback(const CreationCallback& create_callback)
    : m_create_callback{create_callback}
    { };

    auto create(int cpunum) noexcept -> std::unique_ptr<ICpuId> override
    {
        return m_create_callback(cpunum);
    }

private:
    CreationCallback m_create_callback;
};

template <typename CreationCallback>
auto MakeCpuIdFactory(const CreationCallback& callback) -> std::unique_ptr<ICpuIdFactory>
{
    return std::make_unique<CpuIdFactoryWithCallback<CreationCallback>>(callback);
}

} // namespace

// TODO: Consider extracting the CreateCpuIdFactory specializations into
// separate translation units to decouple the concrete types.

template <>
auto CreateCpuIdFactory(const CpuIdDefaultConfig &) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([](int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdDefault>(cpunum); });
}

template <>
auto CreateCpuIdFactory(const CpuIdDeviceConfig &config) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([config](int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdDevice>(cpunum, config.method); });
}

template <>
auto CreateCpuIdFactory(const CpuIdNativeConfig &) noexcept -> std::unique_ptr<ICpuIdFactory>
{
    return MakeCpuIdFactory([](int cpunum) -> std::unique_ptr<ICpuId>
                            { return std::make_unique<CpuIdNative>(cpunum); });
}

}
