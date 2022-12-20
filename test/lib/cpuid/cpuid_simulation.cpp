#include "cpuid/cpuid_simulation.h"

namespace rjcp::cpuid {

auto CpuIdSimulation::GetCpuId(std::uint32_t eax, std::uint32_t ecx) const noexcept -> const CpuIdRegister
{
    auto processor = m_tree->GetProcessor(m_cpunum);
    if (processor == nullptr) return CpuIdRegister{};

    auto leaf = processor->GetLeaf(eax, ecx);
    if (leaf == nullptr) return CpuIdRegister{};

    return *leaf;
}

}
