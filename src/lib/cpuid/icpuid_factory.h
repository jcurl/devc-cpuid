#ifndef RJCP_CPUID_ICPUID_FACTORY_H
#define RJCP_CPUID_ICPUID_FACTORY_H

#include "cpuid/icpuid.h"
#include "cpuid/icpuid_config.h"
#include "cpuid/cpuid_device.h"

#include <memory>

namespace rjcp::cpuid {

class ICpuIdFactory
{
public:
    /**
     * @brief Creates a CPUID object.
     *
     * @param cpunum The CPU object to create for.
     * @return ICpuId The object that can be used to get CPUID information.
     */
    virtual auto create(unsigned int cpunum) noexcept -> std::unique_ptr<ICpuId> = 0;

    /**
     * @brief Get the number of threads (CPUs)
     *
     * @return unsigned int The number of threads.
     */
    virtual auto threads() const -> unsigned int = 0;

    /**
     * @brief Destroy the ICpuIdFactory object
     *
     */
    virtual ~ICpuIdFactory() = default;
};

}

#endif
