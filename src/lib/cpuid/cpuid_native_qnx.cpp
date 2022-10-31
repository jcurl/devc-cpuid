#include "cpuid/cpuid_native.h"

#ifdef __QNXNTO__

#include <sys/neutrino.h>

namespace rjcp::cpuid {

auto CpuIdNative::GetCpuId(std::int32_t eax, std::int32_t ecx) noexcept -> const CpuIdRegister
{
    unsigned int runmask = 1 << m_cpunum;
    int result = ThreadCtl(_NTO_TCTL_RUNMASK_GET_AND_SET, &runmask);
    if (result == -1) {
        // Can't set the affinity, so return the default set.
        return CpuIdRegister{};
    }

    CpuIdRegister cpureg = GetCpuIdCurrentThread(eax, ecx);

    // Restore the thread affinity.
    ThreadCtl(_NTO_TCTL_RUNMASK_GET_AND_SET, &runmask);

    return cpureg;
}

}

#endif
