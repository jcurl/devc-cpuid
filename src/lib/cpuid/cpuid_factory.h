#ifndef RJCP_LIB_CPUID_CPUID_FACTORY_H
#define RJCP_LIB_CPUID_CPUID_FACTORY_H

#include "cpuid/icpuid.h"
#include "cpuid/icpuid_factory.h"

#include "cpuid/cpuid_device_config.h"

#include <memory>

namespace rjcp::cpuid {

/**
 * @brief Template class for factories creating objects of type ICpuId.
 *
 * @tparam T The configuration object, which is also used to choose the object
 * to build from the factory's create() method.
 */
template<typename T>
class CpuIdFactory : public ICpuIdFactory
{
public:
    CpuIdFactory() noexcept;

    CpuIdFactory(T& config) noexcept;

    /**
     * @brief Create the default CPUID object.
     *
     * @param cpunum The CPU object to create for.
     * @return ICpuId The CPUID object.
     */
    auto create(int cpunum) noexcept -> std::unique_ptr<ICpuId>;
};

/**
 * @brief Specialization for the CpuIdDeviceConfig
 *
 * @tparam
 */
template<>
class CpuIdFactory<CpuIdDeviceConfig> : public ICpuIdFactory
{
public:
    CpuIdFactory() noexcept;

    CpuIdFactory(CpuIdDeviceConfig& config) noexcept;

    auto create(int cpunum) noexcept -> std::unique_ptr<ICpuId> override;

private:
    DeviceAccessMethod m_method{DeviceAccessMethod::seek};
};

}

#endif
