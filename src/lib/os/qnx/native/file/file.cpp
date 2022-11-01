#include "os/qnx/native/file/file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

namespace rjcp::os::qnx::native::file {

auto open(const std::string& fileName) noexcept -> expected<FileHandle>
{
    return open(fileName, 0);
}

auto open(const std::string& fileName, int flags) noexcept -> expected<FileHandle>
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg) - Systems programming
    int fd = ::open(fileName.c_str(), flags | O_CLOEXEC);
    if (fd == -1)
        return stdext::make_unexpected(errno);

    return FileHandle{fd};
}

auto read(FileHandle& fd, std::vector<std::uint8_t>& buf, std::size_t count) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    std::size_t readlen = std::min(count, buf.size());
    ssize_t bytes = ::read(fd.Get(), &buf[0], readlen);
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

auto read(FileHandle& fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);
    if (offset > buf.size())
        return stdext::make_unexpected(EINVAL);
    if (offset == buf.size())
        return 0;

    std::size_t readlen = std::min(count, buf.size() - offset);
    ssize_t bytes = ::read(fd.Get(), &buf[offset], readlen);
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

auto read(FileHandle& fd, uint8_t* buf, std::size_t count) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);
    ssize_t bytes = ::read(fd.Get(), buf, count);
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

/**
 * @brief Converts a C++ `std::size_t` which is an unsigned integer (usually a
 * long) to the OS type.
 *
 * Conversions are done assuming 2's complement (Posix requirement) and the most
 * significant bit is converted to a negative seek value. For using the CPUID
 * module, this is exactly what we want, as the ECX is the upper 32-bits when
 * seeking.
 *
 * It is expected then that `lseek` be used with `SEEK_SET`.
 *
 * @tparam T The Posix type to convert to.
 * @param seek The value to seek given by a C++ method.
 * @return T The result to give to the Posix call.
 */
template<typename T>
auto coffset(std::size_t seek) -> T
{
    constexpr int min_off_size = 8;    // Must be at least 64-bits
    if constexpr (sizeof(std::size_t) == sizeof(T) && sizeof(T) >= min_off_size)
        return static_cast<T>(seek);
    else {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        static_assert(
            sizeof(std::size_t) == sizeof(T) && sizeof(T) >= min_off_size,
            "std::size_t and T must be of same size and 64-bit (i.e. enable large file support)");
        return 0;     // Will never get here.
    }
}

auto lseek(FileHandle &fd, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    off_t offset = ::lseek(fd.Get(), coffset<off_t>(seek), SEEK_SET);
    if (offset == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(offset);
}

auto lseek64(FileHandle &fd, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    off64_t offset = ::lseek64(fd.Get(), coffset<off64_t>(seek), SEEK_SET);
    if (offset == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(offset);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    std::size_t readlen = std::min(count, buf.size());
    ssize_t bytes = ::pread(fd.Get(), &buf[0], readlen, coffset<off_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    std::size_t readlen = std::min(count, buf.size() - offset);
    ssize_t bytes = ::pread(fd.Get(), &buf[offset], readlen, coffset<off_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread(FileHandle &fd, uint8_t *buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);
    ssize_t bytes = ::pread(fd.Get(), buf, count, coffset<off_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread64(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    std::size_t readlen = std::min(count, buf.size());

    ssize_t bytes = ::pread64(fd.Get(), &buf[0], readlen, coffset<off64_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread64(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    std::size_t readlen = std::min(count, buf.size() - offset);

    ssize_t bytes = ::pread64(fd.Get(), &buf[offset], readlen, coffset<off64_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto pread64(FileHandle &fd, uint8_t *buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>
{
    if (!fd)
        return stdext::make_unexpected(EINVAL);

    ssize_t bytes = ::pread64(fd.Get(), buf, count, coffset<off64_t>(seek));
    if (bytes == -1)
        return stdext::make_unexpected(errno);

    return static_cast<std::size_t>(bytes);
}

}
