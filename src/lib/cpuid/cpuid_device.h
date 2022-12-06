#ifndef RJCP_LIB_CPUID_CPUID_DEVICE_H
#define RJCP_LIB_CPUID_CPUID_DEVICE_H

#include "cpuid/icpuid.h"
#include "os/qnx/native/file/file.h"

namespace rjcp::cpuid {

enum class DeviceAccessMethod
{
    seek,
    pread
};

/**
 * @brief Query the CPU via the /dev/cpu/N/cpuid device path.
 *
 */
class CpuIdDevice final : public ICpuId
{
public:
    /**
     * @brief Construct a new CpuId Device object
     *
     * @param cpunum The CPU number to configure for.
     * @param method How to access the device using lseek64/read or pread64.
     */
    CpuIdDevice(unsigned int cpunum, DeviceAccessMethod method = DeviceAccessMethod::seek) noexcept;

    /**
     * @brief Get the CPUID for the given EAX and ECX registers.
     *
     * This may result in a change of the CPU that the current thread is running
     * on. If the thread cannot be set properly, the result will be invalid.
     *
     * @param eax The major leaf (EAX register) to query.
     * @param ecx The minor leaf (ECX register) to query.
     * @return CpuIdRegister The result of the query.
     */
    auto GetCpuId(std::uint32_t eax, std::uint32_t ecx) noexcept -> const CpuIdRegister override;

private:
    os::qnx::native::file::FileHandle m_device{};
    DeviceAccessMethod m_method;
};

}

#endif
