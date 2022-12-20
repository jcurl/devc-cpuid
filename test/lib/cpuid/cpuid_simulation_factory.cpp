#include "cpuid/icpuid_factory.h"
#include "cpuid/cpuid_factory.h"
#include "cpuid/cpuid_simulation_config.h"
#include "cpuid/tree/cpuid_tree.h"

#include "cpuid/cpuid_default.h"
#include "cpuid/cpuid_simulation.h"

#include <memory>

namespace rjcp::cpuid {

class CpuIdSimulationFactory : public ICpuIdFactory
{
public:
    CpuIdSimulationFactory(std::shared_ptr<tree::CpuIdTree> tree)
    : m_tree{std::move(tree)}
    { }

    auto create(unsigned int cpunum) noexcept -> std::unique_ptr<ICpuId> override
    {
        return std::make_unique<CpuIdSimulation>(cpunum, m_tree);
    }

    auto threads() const -> unsigned int override
    {
        return m_tree->Size();
    }

private:
    std::shared_ptr<tree::CpuIdTree> m_tree;
};

template<>
auto CreateCpuIdFactory(const CpuIdSimulationConfig& config) noexcept -> std::unique_ptr<ICpuIdFactory> {
    return std::make_unique<CpuIdSimulationFactory>(config.GetTree());
}

}
