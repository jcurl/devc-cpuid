#ifndef RJCP_LIB_CPUID_CPUID_SIMULATION_CONFIG_H
#define RJCP_LIB_CPUID_CPUID_SIMULATION_CONFIG_H

#include "cpuid/icpuid.h"
#include "cpuid/icpuid_config.h"
#include "cpuid/tree/cpuid_tree.h"

#include <memory>

namespace rjcp::cpuid {

/**
 * @brief Configuration providing the predetermined CPUID information.
 *
*/
class CpuIdSimulationConfig : public ICpuIdConfig
{
public:
    /**
     * @brief Construct a new CpuId Simulation Config object.
     *
     * Constructs internally a new tree::CpuIdTree object, which can be accessed
     * via the method Tree().
     */
    CpuIdSimulationConfig()
    : m_tree{std::make_shared<tree::CpuIdTree>()}
    { }

    /**
     * @brief Construct a new CpuId Simulation Config object
     *
     * Given a constant l-value reference, copy to a new internal shared object.
     * That new copy is now available by GetTree().
     *
     * @param tree The CPUID tree to copy.
     */
    CpuIdSimulationConfig(const tree::CpuIdTree& tree)
    : m_tree{std::make_shared<tree::CpuIdTree>(tree)}
    { }

    /**
     * @brief Construct a new CpuId Simulation Config object
     *
     * Given an r-value reference, create an internal shared object and move it
     * there.
     *
     * @param tree The CPUID tree to copy.
     */
    CpuIdSimulationConfig(tree::CpuIdTree&& tree)
    : m_tree{std::make_shared<tree::CpuIdTree>(std::move(tree))}
    { }

    /**
     * @brief Construct a new CpuId Simulation Config object.
     *
     * Constructs the CpuIdSimulation object. The tree given can be accessed via
     * the Tree() method. This is a shared object, so that it can be updated in
     * different places.
     *
     * @param tree The Tree of all CPUID object elements that should be
     * simulated.
     */
    CpuIdSimulationConfig(std::shared_ptr<tree::CpuIdTree> tree)
    : m_tree{std::move(tree)}
    { }

    /**
     * @brief Gets the CPUID tree, that it can be manipulated.
     *
     * @return std::shared_ptr<tree::CpuIdTree> The CPUID tree object.
     */
    auto GetTree() const -> std::shared_ptr<tree::CpuIdTree> {
        return m_tree;
    }

private:
    std::shared_ptr<tree::CpuIdTree> m_tree;
};

}

#endif
