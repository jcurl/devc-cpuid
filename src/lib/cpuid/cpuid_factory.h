#ifndef RJCP_LIB_CPUID_CPUID_FACTORY_H
#define RJCP_LIB_CPUID_CPUID_FACTORY_H

#include "cpuid/icpuid.h"
#include "cpuid/icpuid_factory.h"
#include "cpuid/cpuid_device_config.h"

#include <memory>

namespace rjcp::cpuid {

/**
 * @brief Function to instantiate a factory with a given factory config that serves as a blueprint.
 *
 * @param config The blueprint configuration object that determines which concrete type will be created.
 */
template<typename Config>
auto CreateCpuIdFactory(const Config& config) noexcept -> std::unique_ptr<ICpuIdFactory>;

}

#endif
