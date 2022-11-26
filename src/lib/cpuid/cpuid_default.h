#ifndef RJCP_LIB_CPUID_CPUID_DEFAULT_H
#define RJCP_LIB_CPUID_CPUID_DEFAULT_H

#include "cpuid/icpuid.h"

namespace rjcp::cpuid {

/**
 * @brief Provide a default implementation for testing purposes.
 *
 */
class CpuIdDefault final : public ICpuId
{
public:
    /**
     * @brief Construct a new Cpu Id Default object.
     *
     * @param cpunum The CPU number to configure for.
     */
    CpuIdDefault(int cpunum) noexcept;

    /**
     * @brief Get the CPUID for the given EAX and ECX registers.
     *
     * This operation always returns an invalid result.
     *
     * @param eax The major leaf (EAX register) to query.
     * @param ecx The minor leaf (ECX register) to query.
     * @return CpuIdRegister The result of the query.
     */
    auto GetCpuId(std::uint32_t eax, std::uint32_t ecx) noexcept -> const CpuIdRegister override;
};

}

#endif
