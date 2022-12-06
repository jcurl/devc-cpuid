#include "cpuid/tree/cpuid_tree.h"

#include <cassert>

namespace rjcp::cpuid::tree {

CpuIdTree::CpuIdTree(CpuIdTree&& tree)
: m_registers{std::move(tree.m_registers)}
{ }

auto CpuIdTree::operator=(CpuIdTree&& tree) -> CpuIdTree&
{
    m_registers = std::move(tree.m_registers);
    return *this;
}

auto CpuIdTree::GetProcessor(unsigned int cpu) -> const CpuIdProcessor*
{
    auto result = m_registers.find(cpu);
    if (result == m_registers.cend()) return nullptr;
    return &(*result).second;
}

template<typename T>
auto CpuIdTree::SetProcessorInternal(unsigned int cpu, T&& tree) -> bool
{
    auto value = m_registers.emplace(
        std::make_pair(cpu, std::forward<T>(tree)));
    return value.second;
}

auto CpuIdTree::SetProcessor(unsigned int cpu, const CpuIdProcessor& tree) -> bool
{
    return SetProcessorInternal(cpu, tree);
}

auto CpuIdTree::SetProcessor(unsigned int cpu, CpuIdProcessor&& tree) -> bool
{
    return SetProcessorInternal(cpu, std::move(tree));
}

auto CpuIdTree::Size() -> std::size_t
{
    return m_registers.size();
}

auto CpuIdTree::IsEmpty() -> bool
{
    return m_registers.size() == 0;
}

auto CpuIdTree::begin() noexcept -> iterator
{
    return m_registers.begin();
}

auto CpuIdTree::end() noexcept -> iterator
{
    return m_registers.end();
}

auto CpuIdTree::cbegin() const noexcept -> const_iterator
{
    return m_registers.cbegin();
}

auto CpuIdTree::cend() const noexcept -> const_iterator
{
    return m_registers.cend();
}

}
