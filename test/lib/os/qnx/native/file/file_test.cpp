#include <gtest/gtest.h>

#include "os/qnx/native/file/file.h"

#include <iostream>
#include <vector>

namespace rjcp::os::qnx::native::file {

TEST(File, OpenRandom)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h);
    ASSERT_TRUE(h.has_value());
    ASSERT_TRUE(h.value());       // FileHandle
    ASSERT_TRUE(*h);              // FileHandle
}

TEST(File, OpenInvalidFile)
{
    auto h = open("/dev/nonexistant");
    ASSERT_FALSE(h);
    ASSERT_NE(h.error(), 0);
    EXPECT_EQ(h.error(), ENOENT);
}

TEST(File, ReadRandomVector)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(h.value(), buffer, 8);   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 8);
}

TEST(File, ReadRandomVectorInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(h, buffer, 8);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, ReadRandomVectorConstrained)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(8);            // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 8 elements.
    auto bytes = read(*h, buffer, 256);        // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 8);
}

TEST(File, ReadRandomVectorOffset)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(*h, buffer, 10, 8);      // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 8);
}

TEST(File, ReadRandomVectorOffsetInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(h, buffer, 2, 8);        // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, ReadRandomVectorOffsetConstrained)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(8);            // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(*h, buffer, 2, 256);     // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 6);
}

TEST(File, ReadRandomVectorOffsetConstrained2)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(8);            // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(*h, buffer, 8, 256);     // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 0);
}

TEST(File, ReadRandomVectorOffsetConstrained3)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(8);            // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(*h, buffer, 10, 256);    // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, ReadRandomBuffer)
{
    auto h = open("/dev/random");
    ASSERT_TRUE(h && *h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(*h, &buffer[0], 8);      // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 8);
}

TEST(File, ReadRandomBufferInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(256);          // NOLINT(cppcoreguidelines-avoid-magic-numbers) - Vector of 256 elements.
    auto bytes = read(h, &buffer[0], 8);       // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, SeekInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    auto offset = lseek(h, 0);
    ASSERT_FALSE(offset);
    ASSERT_EQ(offset.error(), EINVAL);
}

TEST(File, SeekCpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    auto offset = lseek(*h, 0);
    ASSERT_TRUE(offset);
    ASSERT_EQ(*offset, 0);

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = read(*h, buffer, buffer.size());
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, Seek64InvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    auto offset = lseek64(h, 0);
    ASSERT_FALSE(offset);
    ASSERT_EQ(offset.error(), EINVAL);
}

TEST(File, Seek64CpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    auto offset = lseek64(*h, 0);
    ASSERT_TRUE(offset);
    ASSERT_EQ(*offset, 0);

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = read(*h, buffer, buffer.size());
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, PreadInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(h, buffer, buffer.size(), 0);
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, PreadCpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(*h, buffer, buffer.size(), 0);
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, PreadOffsetInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(h, buffer, 16, 16, 0);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, PreadCpuIdDeviceOffset)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(*h, buffer, 16, 16, 0); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, PreadBufferInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(h, &buffer[0], 16, 0);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, PreadBufferCpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread(*h, &buffer[16], 16, 0); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, Pread64InvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(h, buffer, buffer.size(), 0);
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, Pread64CpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(16);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(*h, buffer, buffer.size(), 0);
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, Pread64OffsetInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(h, buffer, 16, 16, 0);// NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, Pread64CpuIdDeviceOffset)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(*h, buffer, 16, 16, 0); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

TEST(File, Pread64BufferInvalidHandle)
{
    FileHandle h{};
    ASSERT_FALSE(h);

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(h, &buffer[0], 16, 0);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_FALSE(bytes);
    ASSERT_EQ(bytes.error(), EINVAL);
}

TEST(File, Pread64BufferCpuIdDevice)
{
    // For this test to pass, you must have the driver running, e.g. on Linux
    // `modins cpuid`
    auto h = open("/dev/cpu/0/cpuid");
    ASSERT_TRUE(h && *h) << "Ensure that the cpuid driver is loaded and has readable permissions - " << strerror(h.error());

    std::vector<uint8_t> buffer(32);           // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    auto bytes = pread64(*h, &buffer[16], 16, 0); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    ASSERT_TRUE(bytes);
    ASSERT_EQ(*bytes, 16);
}

}
