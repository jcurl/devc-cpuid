#include "cpuid/cpuid_register.h"

namespace rjcp::cpuid {

constexpr std::uint32_t Invalid = 0xFFFFFFFF;

CpuIdRegister::CpuIdRegister() noexcept
    : m_InEax{Invalid}, m_InEcx{Invalid}, m_Eax(0), m_Ebx(0), m_Ecx(0), m_Edx(0), m_IsValid{false}
{ }

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CpuIdRegister::CpuIdRegister(std::uint32_t ieax, std::uint32_t iecx, std::uint32_t eax, std::uint32_t ebx, std::uint32_t ecx, std::uint32_t edx) noexcept
    : m_InEax{ieax}, m_InEcx{iecx}, m_Eax(eax), m_Ebx(ebx), m_Ecx(ecx), m_Edx(edx), m_IsValid{true}
{ }

auto CpuIdRegister::InEax() const noexcept -> std::uint32_t
{
    return m_InEax;
}

auto CpuIdRegister::InEcx() const noexcept -> std::uint32_t
{
    return m_InEcx;
}

auto CpuIdRegister::Eax() const noexcept -> std::uint32_t
{
    return m_Eax;
}

auto CpuIdRegister::Ebx() const noexcept -> std::uint32_t
{
    return m_Ebx;
}

auto CpuIdRegister::Ecx() const noexcept -> std::uint32_t
{
    return m_Ecx;
}

auto CpuIdRegister::Edx() const noexcept -> std::uint32_t
{
    return m_Edx;
}

auto CpuIdRegister::IsValid() const noexcept -> bool
{
    return m_IsValid;
}

}
