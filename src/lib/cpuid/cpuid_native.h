#ifndef RJCP_LIB_CPUID_CPUID_NATIVE_H
#define RJCP_LIB_CPUID_CPUID_NATIVE_H

#include "cpuid/icpuid.h"

namespace rjcp::cpuid {

/**
 * @brief Query the CPU using the CPUID instruction.
 *
 */
class CpuIdNative final : public ICpuId
{
public:
    /**
     * @brief Construct a new Cpu Id Native object.
     *
     * @param cpunum The CPU number to configure for.
     */
    CpuIdNative(unsigned int cpunum) noexcept;

    /**
     * @brief Get the CPUID for the given EAX and ECX registers.
     *
     * This may result in a change of the CPU that the current thread is running
     * on. If the thread cannot be set properly, the result will be invalid.
     *
     * @param eax The major leaf (EAX register) to query.
     * @param ecx The minor leaf (ECX register) to query.
     * @return CpuIdRegister The result of the query.
     */
    auto GetCpuId(std::uint32_t eax, std::uint32_t ecx) const noexcept -> const CpuIdRegister override;

private:
    /**
     * @brief Get the CPUID for the given EAX and ECX registers on the current
     * thread.
     *
     * This method is a helper to call the native CPU instruction and is
     * specific only to this implementation.
     *
     * @param eax The major leaf (EAX register) to query.
     * @param ecx The minor leaf (ECX register) to query.
     * @return CpuIdRegister The result of the query.
     */
    static auto GetCpuIdCurrentThread(std::uint32_t eax, std::uint32_t ecx) noexcept -> const CpuIdRegister;

    unsigned int m_cpunum;
};

}

#endif
