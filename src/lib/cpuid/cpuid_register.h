#ifndef RJCP_LIB_CPUID_CPUID_REGISTER_H
#define RJCP_LIB_CPUID_CPUID_REGISTER_H

#include <cstdint>

namespace rjcp::cpuid {

/**
 * @brief Represents the result of executing the CPUID instruction.
 *
 */
class CpuIdRegister
{
public:
    /**
     * @brief Construct a new Cpu Id Register object with default values of zero.
     *
     */
    CpuIdRegister() noexcept;

    /**
     * @brief Construct a new Cpu Id Register object
     *
     * @param ieax The input EAX register
     * @param iecx The input ECX register
     * @param eax The result of the CPUID instruction EAX register
     * @param ebx The result of the CPUID instruction EBX register
     * @param ecx The result of the CPUID instruction ECX register
     * @param edx The result of the CPUID instruction EDX register
     */
    CpuIdRegister(std::int32_t ieax, std::int32_t iecx, std::int32_t eax, std::int32_t ebx, std::int32_t ecx, std::int32_t edx) noexcept;

    /**
     * @brief Indicates if the register data is valid.
     *
     * The register data might not be valid if the CPUID instruction cannot be
     * run on the given CPU.
     *
     * @return true The data is valid.
     * @return false The data is not valid.
     */
    auto IsValid() const noexcept -> bool;

    /**
     * @brief The input CPUID EAX register.
     *
     * @return std::int32_t The value used to get the results.
     */
    auto InEax() const noexcept -> std::int32_t;

    /**
     * @brief The input CPUID ECX register
     *
     * @return std::int32_t The value used to get the results.
     */
    auto InEcx() const noexcept -> std::int32_t;

    /**
     * @brief The result of the CPUID instruction EAX register
     *
     * @return std::int32_t The EAX register.
     */
    auto Eax() const noexcept -> std::int32_t;

    /**
     * @brief The result of the CPUID instruction EBX register
     *
     * @return std::int32_t The EBX register.
     */
    auto Ebx() const noexcept -> std::int32_t;

    /**
     * @brief The result of the CPUID instruction ECX register
     *
     * @return std::int32_t The ECX register.
     */
    auto Ecx() const noexcept -> std::int32_t;

    /**
     * @brief The result of the CPUID instruction EDX register
     *
     * @return std::int32_t The EDX register.
     */
    auto Edx() const noexcept -> std::int32_t;

private:
    std::int32_t m_InEax;
    std::int32_t m_InEcx;
    std::int32_t m_Eax;
    std::int32_t m_Ebx;
    std::int32_t m_Ecx;
    std::int32_t m_Edx;
    bool m_IsValid;
};

}

#endif
