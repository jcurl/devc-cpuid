#include "cpuid/tree/cpuid_processor.h"

namespace rjcp::cpuid::tree {

CpuIdProcessor::CpuIdProcessor(CpuIdProcessor&& tree)
: m_node(std::move(tree.m_node))
{ }

auto CpuIdProcessor::operator=(CpuIdProcessor&& other) -> CpuIdProcessor&
{
    m_node = std::move(other.m_node);
    return *this;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto CpuIdProcessor::GetLeaf(std::uint32_t eax, std::uint32_t ecx) const -> const CpuIdRegister*
{
    auto result = m_node.find(CpuIdKey{eax, ecx});
    if (result == m_node.cend()) {
        return nullptr;
    }

    return &(*result).second;
}

template<typename T>
auto CpuIdProcessor::AddLeafInternal(T&& cpureg) -> bool
{
    auto value = m_node.emplace(
        std::make_pair(CpuIdKey(cpureg.InEax(), cpureg.InEcx()), std::forward<T>(cpureg)));
    return value.second;
}

auto CpuIdProcessor::AddLeaf(const CpuIdRegister& cpureg) -> bool
{
    return AddLeafInternal(cpureg);
}

auto CpuIdProcessor::AddLeaf(CpuIdRegister&& cpureg) -> bool
{
    return AddLeafInternal(std::move(cpureg));
}

auto CpuIdProcessor::Size() const -> std::size_t
{
    return m_node.size();
}

auto CpuIdProcessor::IsEmpty() const -> bool
{
    return m_node.size() == 0;
}

auto CpuIdProcessor::begin() noexcept -> iterator
{
    return m_node.begin();
}

auto CpuIdProcessor::end() noexcept -> iterator
{
    return m_node.end();
}

auto CpuIdProcessor::cbegin() const noexcept -> const_iterator
{
    return m_node.cbegin();
}

auto CpuIdProcessor::cend() const noexcept -> const_iterator
{
    return m_node.cend();
}

}
