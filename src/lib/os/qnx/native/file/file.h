#ifndef RJCP_LIB_OS_QNX_NATIVE_FILE_H
#define RJCP_LIB_OS_QNX_NATIVE_FILE_H

#include "os/qnx/native/unique_handle.h"
#include "stdext/expected.h"

#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <string>
#include <vector>

/**
 * @brief Methods that abstract the datatypes of file handling on a Posix
 * system. Implementations take into account the differences between Operating
 * Systems and present a consistent C++ API for as needed functions.
 *
 * The goal is that upper layers deal with C++ types, not different types from
 * the underlying OS. One example of this is the offset, which under Linux is
 * `off_t`, but may be 32-bit or 64-bit. For the 64-bit Posix calls, Linux uses
 * `loff_t` and other OSes might use `off64_t` (usually signed long). For C++,
 * this should be the type `std::size_t` (unsigned long)
 */
namespace rjcp::os::qnx::native::file {

/**
 * @brief The traits that know what a Posix file handle is.
 *
 */
struct FileHandleTraits : UniqueHandleTraits<int>
{
    static auto Invalid() noexcept -> Type
    {
        return -1;
    }

    static void Close(Type value) noexcept
    {
        close(value);
    }
};

using FileHandle = UniqueHandle<FileHandleTraits>;

// A return type, where T is the return value, and the int is the `errno`.
template<typename T>
using expected = stdext::expected<T, int>;

/**
 * @brief Opens the file with default flags
 *
 * @param fileName The name of the file to open.
 * @return FileHandle A file handle, that when it goes out of scope, closes the
 * file.
 */
auto open(const std::string& fileName) noexcept -> expected<FileHandle>;

/**
 * @brief Opens the file with the given flags.
 *
 * To close the file, you need to destroy the handle, or you can reset it.
 *
 * @param fileName The name of the file to open.
 * @param flags The flags to use when opening the file.
 * @return FileHandle A file handle, that when it goes out of scope, closes the
 * file.
 */
auto open(const std::string& fileName, int flags) noexcept -> expected<FileHandle>;

/**
 * @brief Read from the file handle into the buffer provided, not more than
 * count bytes.
 *
 * When passing a vector, the data is written to the start, and will not exceed
 * the size of the vector buffer.
 *
 * @param fd The file handle.
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @return std::size_t The number of bytes read.
 */
auto read(FileHandle& fd, std::vector<std::uint8_t>& buf, std::size_t count) noexcept -> expected<std::size_t>;

/**
 * @brief Read from the file handle into the buffer provided, not more than
 * count bytes.
 *
 * @param fd The file handle.
 * @param buf The buffer to put the results.
 * @param offset The offset in the buffer to put the results.
 * @param count The maximum number of bytes.
 * @return int The number of bytes read, or -1 on error.
 */
auto read(FileHandle& fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count) noexcept -> expected<std::size_t>;

/**
 * @brief Read from the file handle into the buffer provided, not more than
 * count bytes.
 *
 * @param fd The file handle.
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @return int The number of bytes read, or -1 on error.
 */
auto read(FileHandle& fd, uint8_t* buf, std::size_t count) noexcept -> expected<std::size_t>;

/**
 * @brief Seeks to a specific position in the file
 *
 * @param fd The file handle
 * @param seek The offset in the file to seek to, from the beginning
 * @return off_t The offset in the file, or -1 on error.
 */
auto lseek(FileHandle &fd, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Seeks to a specific position in the large file
 *
 * @param fd The file handle
 * @param seek The offset in the file to seek to, from the beginning
 * @return off64_t The offset in the file, or -1 on error.
 */
auto lseek64(FileHandle &fd, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param offset The offset in the buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread(FileHandle &fd, uint8_t *buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread64(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param offset The offset in the buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread64(FileHandle &fd, std::vector<std::uint8_t>& buf, std::size_t offset, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

/**
 * @brief Reads from the file at a specific position
 *
 * @param fd The file handle
 * @param buf The buffer to put the results.
 * @param count The maximum number of bytes.
 * @param seek The offset in the file to seek to, from the beginning
 * @return int The number of bytes read, or -1 on error.
 */
auto pread64(FileHandle &fd, uint8_t *buf, std::size_t count, std::size_t seek) noexcept -> expected<std::size_t>;

}

#endif
