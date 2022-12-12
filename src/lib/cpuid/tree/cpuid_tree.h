#ifndef RJCP_LIB_CPUID_TREE_CPUID_TREE_H
#define RJCP_LIB_CPUID_TREE_CPUID_TREE_H

#include "cpuid/tree/cpuid_processor.h"

#include <map>

namespace rjcp::cpuid::tree {

class CpuIdTree
{
private:
    using CpuIdMap = std::map<unsigned int, CpuIdProcessor>;

public:
    /**
     * @brief Construct a new CPUID Tree object.
     *
     */
    CpuIdTree() = default;

    /**
     * @brief Construct a new CPUID Tree object from another object (copy).
     *
     * @param tree The object to copy from.
     */
    CpuIdTree(const CpuIdTree& tree) = default;

    /**
     * @brief Construct a new CPUID Tree object from another object (move).
     *
     * @param tree The object to move from.
     */
    CpuIdTree(CpuIdTree&& tree);

    /**
     * @brief Destroy the CPUID Tree object
     *
     */
    virtual ~CpuIdTree() = default;

    /**
     * @brief Copy Assignment operator.
     *
     * @param other The other CPUID Tree object to copy from.
     * @return CpuIdTree& The reference to this object.
     */
    auto operator=(const CpuIdTree& other) -> CpuIdTree& = default;

    /**
     * @brief Move Assignment operator.
     *
     * @param other The other CPUID Tree object to move from.
     * @return CpuIdTree& The reference to this object.
     */
    auto operator=(CpuIdTree&& other) -> CpuIdTree&;

    /**
     * @brief Get the Processor object for the given CPU.
     *
     * Given a CPU number, get the processor object. You should not call to get
     * the processor object for negative values of cpu.
     *
     * @param cpu The CPU to get the processor object for.
     * @return CpuIdProcessor* The processor object that exists, or nullptr if
     * it doesn't exist,
     */
    auto GetProcessor(unsigned int cpu) const -> const CpuIdProcessor*;

    /**
     * @brief Set the Processor object for the given CPU.
     *
     * @param cpu The CPU to set the processor object for.
     * @param tree The object to set.
     * @return true The object was set.
     * @return false The object already exists and was not set.
     */
    auto SetProcessor(unsigned int cpu, const CpuIdProcessor& tree) -> bool;

    /**
     * @brief Set the Processor object for the given CPU.
     *
     * @param cpu The CPU to set the processor object for.
     * @param tree The object to set.
     * @return true The object was set.
     * @return false The object already exists and was not set.
     */
    auto SetProcessor(unsigned int cpu, CpuIdProcessor&& tree) -> bool;

    /**
     * @brief Get the number of processors defined in this data structure.
     *
     * @return int The number of processors defined in this data structure.
     */
    auto Size() const noexcept -> std::size_t;

    /**
     * @brief Test if this data structure has any processors defined.
     *
     * @return true There is at least one processor defined in this data
     * structure.
     * @return false There are no processors defined in this data structure.
     */
    auto IsEmpty() const noexcept -> bool;

    /**
     * @brief The type for the iterator.
     *
     * Use this type to know what the iterator type is, instead of relying on
     * private details.
     */
    using iterator = CpuIdMap::iterator;

    /**
     * @brief The type for the constant iterator.
     *
     * Use this type to know what the constant iterator type is, instead of
     * relying on private details.
     */
    using const_iterator = CpuIdMap::const_iterator;

    /**
     * @brief Iterator to the first element.
     *
     * @return iterator The iterator to the first element.
     */
    auto begin() noexcept -> iterator;

    /**
     * @brief Iterator following the last element.
     *
     * @return iterator The iterator following the last element.
     */
    auto end() noexcept -> iterator;

    /**
     * @brief Constant iterator to the first element.
     *
     * @return const_iterator The constant iterator to the first element.
     */
    auto cbegin() const noexcept -> const_iterator;

    /**
     * @brief Constant iterator following the last element.
     *
     * @return const_iterator The constant iterator following the last element.
     */
    auto cend() const noexcept -> const_iterator;

private:
    CpuIdMap m_registers{};

    template<typename T>
    auto SetProcessorInternal(unsigned int cpu, T&& tree) -> bool;
};

}

#endif
