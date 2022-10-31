#include "cpuid/cpuid_native.h"

#include <pthread.h>

namespace rjcp::cpuid {

auto CpuIdNative::GetCpuId(std::int32_t eax, std::int32_t ecx) noexcept -> const CpuIdRegister
{
    pthread_t thread = pthread_self();
    cpu_set_t current;
    cpu_set_t cpuset;

    if (0 != pthread_getaffinity_np(thread, sizeof(current), &current)) {
        // Can't set the affinity, so return the default set.
        return CpuIdRegister{};
    }

    CPU_ZERO(&cpuset);
    CPU_SET(m_cpunum, &cpuset);
    if (0 != pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset)) {
        // Can't set the affinity, so return the default set.
        return CpuIdRegister{};
    }

    CpuIdRegister cpureg = GetCpuIdCurrentThread(eax, ecx);

    pthread_setaffinity_np(thread, sizeof(current), &current);

    return cpureg;
}

}
