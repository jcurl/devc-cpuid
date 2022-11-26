#include "cpuid/cpuid_native.h"

#include <pthread.h>

namespace rjcp::cpuid {

CpuIdNative::CpuIdNative(int cpunum) noexcept
    : m_cpunum(cpunum) { }

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto CpuIdNative::GetCpuIdCurrentThread(std::uint32_t eax, std::uint32_t ecx) noexcept -> const CpuIdRegister
{
    std::uint32_t oeax, oebx, oecx, oedx; // NOLINT(cppcoreguidelines-init-variables) - Initialized by the cpuid instruction.

    __asm__ __volatile__ ("cpuid"
        : "=a"(oeax), "=b"(oebx), "=c"(oecx), "=d"(oedx)
        : "a"(eax), "c"(ecx)
        );

    return CpuIdRegister{eax, ecx, oeax, oebx, oecx, oedx};
}

}
