#ifndef RJCP_LIB_OS_QNX_NATIVE_UNIQUE_HANDLE_TRAITS_H
#define RJCP_LIB_OS_QNX_NATIVE_UNIQUE_HANDLE_TRAITS_H

namespace rjcp::os::qnx::native {

/**
 * @brief Defines a trait for the UniqueHandle class.
 *
 * Defines the traits on how to free the object handled by the trait. The field
 * Invalid() defines what the invalid type should be. For pointers, this is
 * normally nullptr. Other types might be integers, such as -1 (like an invalid
 * file handle).
 *
 * Every trait defaults to the invalid value being nullptr. You must define the
 * method `static void Close(Type value) noexcept` which describes how to free
 * the object when it goes out of scope or is deleted.
 *
 * @tparam T The user datatype that should be handled
 */
template <typename T>
struct UniqueHandleTraits
{
public:
    using Type = T;

    /**
     * @brief The value of the object when it is invalid.
     *
     * @return Type
     */
    static auto Invalid() noexcept -> Type
    {
        return nullptr;
    };

    // static void Close(Type value) noexcept;
};

}

#endif
