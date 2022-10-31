#include "cpuid/cpuid_default.h"

namespace rjcp::cpuid {

CpuIdDefault::CpuIdDefault(int /* cpunum */) noexcept
{
    /* Nothing to do, we're CPU independent for default data */
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto CpuIdDefault::GetCpuId(std::int32_t /* eax */, std::int32_t /* ecx */) noexcept -> const CpuIdRegister
{
    return CpuIdRegister{};
}

}
