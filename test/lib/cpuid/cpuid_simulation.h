#ifndef RJCP_LIB_CPUID_CPUID_SIMULATION_H
#define RJCP_LIB_CPUID_CPUID_SIMULATION_H

#include "cpuid/icpuid.h"
#include "cpuid/tree/cpuid_processor.h"
#include "cpuid/tree/cpuid_tree.h"

#include <memory>
#include <utility>

namespace rjcp::cpuid {

class CpuIdSimulation final : public ICpuId
{
public:
    CpuIdSimulation(unsigned int cpunum)
    : m_cpunum{cpunum}, m_tree{std::make_shared<tree::CpuIdTree>()}
    { }

    CpuIdSimulation(unsigned int cpunum, std::shared_ptr<tree::CpuIdTree> tree)
    : m_cpunum{cpunum}, m_tree{std::move(tree)}
    { }

    auto GetCpuId(std::uint32_t eax, std::uint32_t ecx) const noexcept -> const CpuIdRegister override;

private:
    unsigned int m_cpunum;
    std::shared_ptr<tree::CpuIdTree> m_tree;
};

}

#endif
