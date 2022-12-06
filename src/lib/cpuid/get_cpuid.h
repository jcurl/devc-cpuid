#ifndef RJCP_CPUID_GET_CPUID_H
#define RJCP_CPUID_GET_CPUID_H

#include "cpuid/icpuid_factory.h"
#include "cpuid/tree/cpuid_tree.h"

#include <memory>

namespace rjcp::cpuid {

auto GetCpuId(ICpuIdFactory& factory) -> std::unique_ptr<tree::CpuIdTree>;

}

#endif
