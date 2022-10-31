#include "cpuid/cpuid_register.h"

namespace rjcp::cpuid {

CpuIdRegister::CpuIdRegister() noexcept
    : m_Eax(0), m_Ebx(0), m_Ecx(0), m_Edx(0), m_IsValid{false}
{ }

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CpuIdRegister::CpuIdRegister(std::int32_t eax, std::int32_t ebx, std::int32_t ecx, std::int32_t edx) noexcept
    : m_Eax(eax), m_Ebx(ebx), m_Ecx(ecx), m_Edx(edx), m_IsValid{true}
{ }

auto CpuIdRegister::Eax() const noexcept -> std::int32_t
{
    return m_Eax;
}

auto CpuIdRegister::Ebx() const noexcept -> std::int32_t
{
    return m_Ebx;
}

auto CpuIdRegister::Ecx() const noexcept -> std::int32_t
{
    return m_Ecx;
}

auto CpuIdRegister::Edx() const noexcept -> std::int32_t
{
    return m_Edx;
}

auto CpuIdRegister::IsValid() const noexcept -> bool
{
    return m_IsValid;
}

}
