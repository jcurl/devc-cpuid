#include <gtest/gtest.h>

#include <cstdlib>

#include "os/qnx/native/unique_handle.h"
#include "os/qnx/native/filehandle_type.h"
#include "os/qnx/native/opaque_type.h"

namespace rjcp::os::qnx::native {

// Used to check how often the trait is closed.
static int closed = 0;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

struct UniqueHandleOpaqueTraits : UniqueHandleTraits<opaque_type_t *>
{
    static void Close(Type value) noexcept
    {
        closed++;
        opaque_destroy(value);
    }
};

TEST(UniqueHandle, CreateOpaque)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
    ASSERT_NE(h.Get(), nullptr);
}

TEST(UniqueHandle, CreateOpaqueBoolTest)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
    ASSERT_TRUE(h);
}

TEST(UniqueHandle, CreateOpaqueInvalid)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h;
    ASSERT_EQ(h.Get(), nullptr);
}

TEST(UniqueHandle, CreateOpaqueInvalidBoolTest)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{};
    ASSERT_FALSE(h);
}

TEST(UniqueHandle, AutoDestroyOpaque)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
        ASSERT_NE(h.Get(), nullptr);
    }

    // Ensures that it was called exactly once when it went out of scope.
    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, MoveConstructor)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
    ASSERT_NE(h.Get(), nullptr);

    UniqueHandle<UniqueHandleOpaqueTraits> h2{std::move(h)};
    ASSERT_EQ(h.Get(), nullptr);    // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
    ASSERT_NE(h2.Get(), nullptr);
}

TEST(UniqueHandle, MoveConstructorDefault)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{};
    ASSERT_EQ(h.Get(), nullptr);

    UniqueHandle<UniqueHandleOpaqueTraits> h2{std::move(h)};
    ASSERT_EQ(h.Get(), nullptr);    // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
    ASSERT_EQ(h2.Get(), nullptr);
}

TEST(UniqueHandle, DetachOpaque)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
    opaque_type_t *old = h.Get();

    opaque_type_t *handle = h.Detach();
    ASSERT_EQ(h.Get(), nullptr);
    ASSERT_FALSE(h);
    ASSERT_EQ(handle, old);

    opaque_destroy(handle);
}

TEST(UniqueHandle, ResetFromInvalidToInvalid)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{};
    ASSERT_FALSE(h);

    bool result = h.Reset(nullptr);
    ASSERT_FALSE(result);
    ASSERT_FALSE(h);
}

TEST(UniqueHandle, ResetFromInvalidToValid)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleOpaqueTraits> h{};
        ASSERT_FALSE(h);

        bool result = h.Reset(opaque_create());
        ASSERT_TRUE(result);
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, ResetFromValidToInvalid)
{
    bool result = false;
    closed = 0;
    {
        UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
        ASSERT_TRUE(h);

        result = h.Reset(nullptr);
        ASSERT_FALSE(result);
        ASSERT_FALSE(h);
    }

    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, ResetFromValidToValid)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
        ASSERT_TRUE(h);

        bool result = h.Reset(opaque_create());
        ASSERT_EQ(closed, 1);
        ASSERT_TRUE(result);
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 2);
}

static auto MoveFunc(UniqueHandle<UniqueHandleOpaqueTraits> i) -> UniqueHandle<UniqueHandleOpaqueTraits>
{
    return i;
}

TEST(UniqueHandle, MoveAssignment)
{
    UniqueHandle<UniqueHandleOpaqueTraits> a1, a2;
    a1 = MoveFunc(UniqueHandle<UniqueHandleOpaqueTraits>(opaque_create()));
    a2 = std::move(a1);

    ASSERT_FALSE(a1);  // NOLINT(bugprone-use-after-move)
    ASSERT_TRUE(a2);
}

TEST(UniqueHandle, SetOpaqueAlreadySet)
{
    UniqueHandle<UniqueHandleOpaqueTraits> h{opaque_create()};
    ASSERT_DEBUG_DEATH({
        h.Set();
    }, "");
}

TEST(UniqueHandle, SetOpaqueNotSet)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleOpaqueTraits> h{};
        ASSERT_NE(h.Set(), nullptr);
        ASSERT_FALSE(h);

        opaque_type_t** ptr = h.Set();
        *ptr = opaque_create();
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 1);
}

struct UniqueHandleFileTraits : UniqueHandleTraits<int>
{
    static auto Invalid() noexcept -> Type
    {
        return -1;
    }

    static void Close(Type value) noexcept
    {
        closed++;
        file_close(value);
    }
};

TEST(UniqueHandle, CreateValidFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
    ASSERT_NE(h.Get(), -1);
}

TEST(UniqueHandle, CreateValidFileHandleBoolTest)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
    ASSERT_TRUE(h);
}

TEST(UniqueHandle, CreateDefaultFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h;
    ASSERT_EQ(h.Get(), -1);
}

TEST(UniqueHandle, CreateDefaultFileHandleBoolTest)
{
    UniqueHandle<UniqueHandleFileTraits> h{};
    ASSERT_FALSE(h);
}

TEST(UniqueHandle, CreateInvalidFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(0)};
    ASSERT_EQ(h.Get(), -1);
}

TEST(UniqueHandle, CreateInvalidFileHandleBoolTest)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(0)};
    ASSERT_FALSE(h);
}

TEST(UniqueHandle, AutoDestroyFileHandle)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
        ASSERT_NE(h.Get(), -1);
    }

    // Ensures that it was called exactly once when it went out of scope.
    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, AutoDestroyInvalidFileHandle)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{file_create(0)};
        ASSERT_EQ(h.Get(), -1);
    }

    // Should not be called, as it was never valid.
    ASSERT_EQ(closed, 0);
}

TEST(UniqueHandle, AutoDestroyDefaultFileHandle)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{};
        ASSERT_EQ(h.Get(), -1);
    }

    // Should not be called, as it was never valid.
    ASSERT_EQ(closed, 0);
}

TEST(UniqueHandle, MoveConstructorFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
    ASSERT_NE(h.Get(), -1);

    UniqueHandle<UniqueHandleFileTraits> h2{std::move(h)};
    ASSERT_EQ(h.Get(), -1);    // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
    ASSERT_NE(h2.Get(), -1);
}

TEST(UniqueHandle, MoveConstructorInvalidFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(0)};
    ASSERT_EQ(h.Get(), -1);

    UniqueHandle<UniqueHandleFileTraits> h2{std::move(h)};
    ASSERT_EQ(h.Get(), -1);    // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
    ASSERT_EQ(h2.Get(), -1);
}

TEST(UniqueHandle, MoveConstructorDefaultFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{};
    ASSERT_EQ(h.Get(), -1);

    UniqueHandle<UniqueHandleFileTraits> h2{std::move(h)};
    ASSERT_EQ(h.Get(), -1);    // NOLINT(bugprone-use-after-move,clang-analyzer-cplusplus.Move)
    ASSERT_EQ(h2.Get(), -1);
}

TEST(UniqueHandle, DetachFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
    int old = h.Get();

    int handle = h.Detach();
    ASSERT_EQ(h.Get(), -1);
    ASSERT_FALSE(h);
    ASSERT_EQ(handle, old);

    file_close(handle);
}

TEST(UniqueHandle, DetachInvalidFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(0)};

    int handle = h.Detach();
    ASSERT_EQ(h.Get(), -1);
    ASSERT_FALSE(h);
    ASSERT_EQ(handle, -1);
}

TEST(UniqueHandle, DetachDefaultFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> h{};

    int handle = h.Detach();
    ASSERT_EQ(h.Get(), -1);
    ASSERT_FALSE(h);
    ASSERT_EQ(handle, -1);
}

TEST(UniqueHandle, ResetFileHandleFromInvalidToInvalid)
{
    UniqueHandle<UniqueHandleFileTraits> h{};
    ASSERT_FALSE(h);

    bool result = h.Reset(-1);
    ASSERT_FALSE(result);
    ASSERT_FALSE(h);
}

TEST(UniqueHandle, ResetFileHandleFromInvalidToValid)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{};
        ASSERT_FALSE(h);

        bool result = h.Reset(file_create(1));
        ASSERT_TRUE(result);
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, ResetFileHandleFromValidToInvalid)
{
    bool result = false;
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
        ASSERT_TRUE(h);

        result = h.Reset(-1);
        ASSERT_FALSE(result);
        ASSERT_FALSE(h);
    }

    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, ResetFileHandleFromValidToValid)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
        ASSERT_TRUE(h);

        bool result = h.Reset(file_create(1));
        ASSERT_EQ(closed, 1);
        ASSERT_TRUE(result);
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 2);
}

static auto MoveFunc(UniqueHandle<UniqueHandleFileTraits> i) -> UniqueHandle<UniqueHandleFileTraits>
{
    return i;
}

TEST(UniqueHandle, MoveAssignmentFileHandle)
{
    UniqueHandle<UniqueHandleFileTraits> a1, a2;
    a1 = MoveFunc(UniqueHandle<UniqueHandleFileTraits>(file_create(1)));
    a2 = std::move(a1);

    ASSERT_FALSE(a1);  // NOLINT(bugprone-use-after-move)
    ASSERT_TRUE(a2);
}

TEST(UniqueHandle, SetFileHandleAlreadySet)
{
    UniqueHandle<UniqueHandleFileTraits> h{file_create(1)};
    ASSERT_DEBUG_DEATH({
        h.Set();
    }, "");
}

TEST(UniqueHandle, SetFileHandleNotSet)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{};
        ASSERT_NE(h.Set(), nullptr);
        ASSERT_FALSE(h);

        int* ptr = h.Set();
        *ptr = file_create(1);
        ASSERT_TRUE(h);
    }
    ASSERT_EQ(closed, 1);
}

TEST(UniqueHandle, SetFileHandleNotSetInvalid)
{
    closed = 0;
    {
        UniqueHandle<UniqueHandleFileTraits> h{};
        ASSERT_NE(h.Set(), nullptr);
        ASSERT_FALSE(h);

        int* ptr = h.Set();
        *ptr = file_create(0);
        ASSERT_FALSE(h);
    }
    ASSERT_EQ(closed, 0);
}

}
