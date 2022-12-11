#ifndef RJCP_LIB_CPUID_ICPUID_H
#define RJCP_LIB_CPUID_ICPUID_H

#include "cpuid/cpuid_register.h"

#include <cstdint>

namespace rjcp::cpuid {

class ICpuId
{
public:
    /**
     * @brief Get the CPU ID registers for a specific EAX, ECX.
     *
     * @param eax The value of EAX to query for.
     * @param ecx The value of ECX to query for.
     * @return CpuIdRegister The results of the query.
     */
    virtual auto GetCpuId(std::uint32_t eax, std::uint32_t ecx) const noexcept -> const CpuIdRegister = 0;

    /**
     * @brief Destroy the ICpuId object
     *
     */
    virtual ~ICpuId() = default;
};

}

#endif
