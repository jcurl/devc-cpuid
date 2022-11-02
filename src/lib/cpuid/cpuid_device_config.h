#ifndef RJCP_CPUID_DEVICE_CONFIG_H
#define RJCP_CPUID_DEVICE_CONFIG_H

#include "cpuid/icpuid_config.h"
#include "cpuid/cpuid_device.h"

namespace rjcp::cpuid {

/**
 * @brief Configuration for the CpuIdDevice class for use with factories.
 *
 */
class CpuIdDeviceConfig : public ICpuIdConfig
{
public:
    CpuIdDeviceConfig(DeviceAccessMethod method = DeviceAccessMethod::seek)
        : method{method}
    { }

    DeviceAccessMethod method;
};

}

#endif
