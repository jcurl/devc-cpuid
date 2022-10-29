#ifndef RJCP_LIB_OS_QNX_NATIVE_UNIQUE_HANDLE_H
#define RJCP_LIB_OS_QNX_NATIVE_UNIQUE_HANDLE_H

#include "os/qnx/native/unique_handle_traits.h"

namespace rjcp::os::qnx::native {

/**
 * @brief UniqueHandle provides a base type that can be used for general opaque
 * types that should be freed with custom free functions.
 *
 * Use a Unique Handle for proper handling (wrapper) of opaque types that have
 * custom deleter functions, and where the allocated value is not always the
 * nullptr.
 *
 * @tparam Traits
 */
template <typename Traits>
class UniqueHandle
{
public:
    using Type = decltype(Traits::Invalid());

    /**
     * @brief Construct a new UniqueHandle object from the OS opaque type.
     *
     * The type of the UniqueHandle is defined by the type of the
     * {@link UniqueHandleTraits} (e.g. the type of the
     * {@link UniqueHandleTraits::Invalid()} method).
     *
     * As the object is expected to be a value type from the Operating System,
     * it is simply copied to this underlying type.
     *
     * @param value The value that should be wrapped. If not provided, this
     * class wraps the invalid value.
     */
    explicit UniqueHandle(Type value = Traits::Invalid()) noexcept
        : m_value(value)
    { }

    UniqueHandle(UniqueHandle const &) = delete;

    auto operator = (UniqueHandle const &) -> UniqueHandle& = delete;

    /**
     * @brief Move Constructor from another UniqueHandle object
     *
     * @param other The other object that should be invalidated and moved here.
     */
    UniqueHandle(UniqueHandle&& other) noexcept
        : m_value(other.Detach())
    { }

    /**
     * @brief Destroy the Unique Handle object
     *
     * This closes the handle using the supplied
     * {@code UniqueHandleTraits::Close()} function.
     */
    ~UniqueHandle() noexcept
    {
        Close();
    }

    /**
     * @brief Detaches the underlying value from this object, invalidating this
     * object.
     *
     * When detaching, the underlying type is returned. The wrapped value is not
     * closed, so detaching the type and not manually freeing will lead to a
     * memory / resource leak.
     *
     * @return Type
     */
    auto Detach() noexcept -> Type
    {
        Type value = m_value;
        m_value = Traits::Invalid();
        return value;
    }

    /**
     * @brief Frees the underlying value and replaces (resets) with the new
     * type.
     *
     * The return bool value can be used in a single line to know if the new
     * value is valid, e.g. in a single line that would allocate the new handle.
     * Regardless of the new value, the old value is closed.
     *
     *   if (handle.Reset(GetHandle())) {
     *     // Handle was allocated successfully
     *   }
     *
     * @param value The new value to replace with, if provided.
     * @return true Indicates this object is valid.
     * @return false Indicates this object is invalid.
     */
    auto Reset(Type value = Traits::Invalid()) noexcept -> bool
    {
        Close();
        m_value = value;
        return static_cast<bool>(*this);
    }

    /**
     * @brief Move assignment operator
     *
     * Moves the underlying value in the {@param other} to this object and
     * invalidate the other object.
     *
     * @param other The object to move to here, invalidating this 'other'
     * object.
     * @return UniqueHandle& The reference to this object.
     */
    auto operator = (UniqueHandle&& other) noexcept -> UniqueHandle&
    {
        if (this != &other) {
            Reset(other.Detach());
        }
        return *this;
    }

    /**
     * @brief Get the underlying value.
     *
     * The underlying value stored is returned. There is no reference counting,
     * nor is the object freed. If this ojbect is deleted, the returned valid is
     * undefined. Do not free the object returned here using other API,
     * undefined behaviour can occur if the object is then freed twice (once by
     * you, the other when this object is deleted).
     *
     * @return Type The underlying value wrapped by this type.
     */
    auto Get() const noexcept -> Type
    {
        return m_value;
    }

    /**
     * @brief Gets the address of the value.
     *
     * Getting the underlying value is useful to overwrite the value stored
     * here. It should be used for the default initialized type (i.e. use the
     * default constructor and do not assign it a value, or assign it only the
     * invalid value as given by the trait).
     *
     * The purpose of this method is to return the address of this value, that
     * functions can set themselves, especially if a function allocates the
     * opaque type not as a return value, but as an out value from the parameter
     * list.
     *
     * Using this property to set the internal value when it is already set is
     * undefined behaviour. Under DEBUG mode, your program will assert.
     *
     * @return Type* The address of the underlying type, that is not assigned
     * yet.
     */
    auto Set() noexcept -> Type*
    {
        assert(!*this);
        return &m_value;
    }

    /**
     * @brief Bool operator to test if the stored underlying type is valid or
     * not.
     *
     * @return true The value is valid, and is not the
     * UniqueHandleTrait::Invalid() value.
     * @return false The value is not valid.
     */
    explicit operator bool() const noexcept
    {
        return m_value != Traits::Invalid();
    }

private:
    Type m_value;

    void Close() noexcept
    {
        if (*this) {
            Traits::Close(m_value);
        }
    }
};

}

#endif
