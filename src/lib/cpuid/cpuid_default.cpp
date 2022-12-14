#include "cpuid/cpuid_default.h"

namespace rjcp::cpuid {

CpuIdDefault::CpuIdDefault(unsigned int /* cpunum */) noexcept
{
    /* Nothing to do, we're CPU independent for default data */
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto CpuIdDefault::GetCpuId(std::uint32_t /* eax */, std::uint32_t /* ecx */) const noexcept -> const CpuIdRegister
{
    return CpuIdRegister{};
}

}
