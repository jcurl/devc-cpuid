#include "cpuid/cpuid_device.h"
#include "os/qnx/native/file/file.h"

#include <cstdint>
#include <sstream>
#include <utility>
#include <vector>

namespace rjcp::cpuid {

CpuIdDevice::CpuIdDevice(unsigned int cpunum, DeviceAccessMethod method) noexcept
    : m_method{method}
{
    std::ostringstream device;
    device << "/dev/cpu/" << cpunum << "/cpuid";

    auto handle = os::qnx::native::file::open(device.str());
    if (handle)
        m_device = std::move(*handle);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto CpuIdDevice::GetCpuId(std::uint32_t eax, std::uint32_t ecx) noexcept -> const CpuIdRegister
{
    if (!m_device) {
        // Device couldn't be opened, so return the default. For example, this
        // CPU doesn't exist, or the driver isn't loaded.
        return CpuIdRegister{};
    }

    std::vector<uint8_t> buffer(16);

    // We provide two different methods for testing. Under QNX, the pread must
    // be handled explicitly and is different to read.
    std::size_t pos = eax | static_cast<std::size_t>(ecx) << 32;
    if (m_method == DeviceAccessMethod::seek) {
        auto seek = os::qnx::native::file::lseek64(m_device, pos);
        if (!seek) {
            // Seeking to the EAX failed.
            return CpuIdRegister{};
        }

        auto bytes = os::qnx::native::file::read(m_device, buffer, buffer.size());
        if (!bytes || *bytes != buffer.size()) {
            // Couldn't read the bytes, or the number of bytes didn't match what we
            // expected.
            return CpuIdRegister();
        }
    } else {
        auto bytes = os::qnx::native::file::pread64(m_device, buffer, buffer.size(), pos);
        if (!bytes || *bytes != buffer.size()) {
            // Couldn't read the bytes, or the number of bytes didn't match what we
            // expected.
            return CpuIdRegister();
        }
    }

    std::uint32_t oeax = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    std::uint32_t oebx = buffer[4] | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24);
    std::uint32_t oecx = buffer[8] | (buffer[9] << 8) | (buffer[10] << 16) | (buffer[11] << 24);
    std::uint32_t oedx = buffer[12] | (buffer[13] << 8) | (buffer[14] << 16) | (buffer[15] << 24);
    return CpuIdRegister{eax, ecx, oeax, oebx, oecx, oedx};
}

}
