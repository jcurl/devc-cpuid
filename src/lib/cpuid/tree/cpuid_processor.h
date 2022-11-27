#ifndef RJCP_LIB_CPUID_TREE_CPUID_PROCESSOR_H
#define RJCP_LIB_CPUID_TREE_CPUID_PROCESSOR_H

#include "cpuid/cpuid_register.h"

#include <cstdint>
#include <map>

namespace rjcp::cpuid::tree {

class CpuIdProcessor
{
private:
    struct CpuIdKey {
        CpuIdKey(std::uint32_t eax, std::uint32_t ecx)
        : eax{eax}, ecx{ecx} { }

        std::uint32_t eax;
        std::uint32_t ecx;
    };

    struct CpuIdKeySort {
        auto operator()(const CpuIdKey& lhs, const CpuIdKey& rhs) const -> bool
        {
            if (lhs.eax > rhs.eax) return false;
            return lhs.eax < rhs.eax || lhs.ecx < rhs.ecx;
        }
    };

    using CpuIdNode = std::map<CpuIdKey, CpuIdRegister, CpuIdKeySort>;

public:
    /**
     * @brief Construct a new CPU register list.
     *
     */
    CpuIdProcessor() = default;

    /**
     * @brief Construct a new CPU register list from another object (copy).
     *
     * @param tree The object to copy from.
     */
    CpuIdProcessor(const CpuIdProcessor& tree) = default;

    /**
     * @brief Construct a new CPU register list from another object (move).
     *
     * @param tree The object to move from.
     */
    CpuIdProcessor(CpuIdProcessor&& tree);

    /**
     * @brief Destroy the CPUID Processor object
     *
     */
    virtual ~CpuIdProcessor() = default;

    /**
     * @brief Copy Assignment operator.
     *
     * @param other The other CPU register list to copy from.
     * @return CpuIdTree& The reference to this object.
     */
    auto operator=(const CpuIdProcessor& other) -> CpuIdProcessor& = default;

    /**
     * @brief Move Assignment operator.
     *
     * @param other The other CPU register list to move from.
     * @return CpuIdTree& The reference to this object.
     */
    auto operator=(CpuIdProcessor&& other) -> CpuIdProcessor&;

    /**
     * @brief Get the Leaf object for the given EAX and ECX value.
     *
     * @param eax The CPUID EAX value.
     * @param ecx The CPUID ECX value.
     * @return CpuIdRegister* The object stored. If it is undefined, then
     * nullptr is returned.
     */
    auto GetLeaf(std::uint32_t eax, std::uint32_t ecx) const -> const CpuIdRegister*;

    /**
     * @brief Add the CPUID register data to this object.
     *
     * Given the CPUID register data, add to this object, that it can be
     * retrieved later.
     *
     * @param cpureg The CPUID register data to add to this object.
     * @return true The object was added.
     * @return false The object already exists and was not added.
     */
    auto AddLeaf(const CpuIdRegister& cpureg) -> bool;

    /**
     * @brief Add the CPUID register data to this object.
     *
     * Given the CPUID register data, add to this object, that it can be
     * retrieved later.
     *
     * @param cpureg The CPUID register data to add to this object.
     * @return true The object was added.
     * @return false The object already exists and was not added.
     */
    auto AddLeaf(CpuIdRegister&& cpureg) -> bool;

    /**
     * @brief Get the number of registers defined in this data structure.
     *
     * @return int The number of registers defined in this data structure.
     */
    auto Size() const -> std::size_t;

    /**
     * @brief Test if this data structure has any CPUID leafs.
     *
     * @return true There is at least one leaf defined in this data structure.
     * @return false There are no leaves defined in this data structure.
     */
    auto IsEmpty() const -> bool;

    /**
     * @brief The type for the iterator.
     *
     * Use this type to know what the iterator type is, instead of relying on
     * private details.
     */
    using iterator = CpuIdNode::iterator;

    /**
     * @brief The type for the constant iterator.
     *
     * Use this type to know what the constant iterator type is, instead of
     * relying on private details.
     */
    using const_iterator = CpuIdNode::const_iterator;

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
    CpuIdNode m_node{};

    template<typename T>
    auto AddLeafInternal(T&& cpureg) -> bool;
};

}

#endif
