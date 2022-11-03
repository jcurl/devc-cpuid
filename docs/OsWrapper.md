# Design and Rationale for the OS Wrappers <!-- omit in toc -->

Systems programming is often done via C interfaces, indeed Posix is implemented
through the system's C library, which wraps around the system calls to the
Operating System itself.

Reasons to abstract the Operating System to a C++ layer is as follows:

* Encapsulate differences between Operating Systems in a small, thin wrapper.
* Wrap resource management items to help avoid free after use, allocation errors
  and avoid common problems such as forgetting to free resources (even if these
  resources are not pointers).
* Emulate missing functionality in one Operating System for another, such as GNU
  extensions.
* Provide a C++ consistent projection of the Operating System. Operating Systems
  programming uses types that might differ to C++, such as `size_t` or
  `ssize_t`, which should be replaced instead with other mechanisms and
  `std::size_t` (but can't be done with a one-to-one wrapper).
* Any code that relies on raw pointers are encapsulated in this layer, making
  auditing of other code easier and safer to use.

- [1. Namespace for OS Wrappers](#1-namespace-for-os-wrappers)
- [2. Unique Handles](#2-unique-handles)
  - [2.1. File Descriptors](#21-file-descriptors)
- [3. Abstracting Operating System Types](#3-abstracting-operating-system-types)
- [4. The C++23 std::expected type](#4-the-c23-stdexpected-type)
- [5. Emulating Layers](#5-emulating-layers)

## 1. Namespace for OS Wrappers

The namespace for the OS wrappers in this project is `rjcp::os::qnx::native`.

## 2. Unique Handles

The C++ `unique_ptr` is an inspiration for safe handling of pointers. Not all
operating system constructs use pointers though, the most notorious under Posix
is the file descriptor, or under Windows the various `HANDLE` types.

Alternatively, some objects when allocated require a specialised function to
free that resource, not necessarily the system library call `free()`, but
instead `close`, or `CloseHandle`.

Taken from a Windows Blog, this project implements a `UniqueHandle` with traits,
as described by [Windows with C++ - Visual C++ 2015 Brings Modern C++ to Legacy
Code](https://learn.microsoft.com/en-us/archive/msdn-magazine/2015/april/windows-with-c-visual-c-2015-brings-modern-c-to-legacy-code)

The Operating System layer in this project can return a `UniqueHandle` wrapping
an Operating System resource (e.g. a file descriptor), provide a uniform way to
know if that handle is valid or not, and when that handle goes out of scope, it
is freed using a custom function.

The code is implemented as templates under `lib/os/qnx/native/unique_handle.h`
and `unique_handle_traits.h`.

### 2.1. File Descriptors

One of the first applications of the `UniqueHandle` is to extend this concept
for file descriptors. These are invalid if it has the value of `-1`, and should
be closed when it moves out of scope. File handles generally should not be
shared to avoid race conditions. While sharing may be desirable in some
situations, it is not required in this project, that device handling is wrapped
by a single class only.

The implementation (and thus mechanism for implementing other unique handles)
is:

1. Create a trait, derived from `UniqueHandleTraits`. For the case of files,
   this is called `FileHandleTraits`. It implements the following `static`
   methods:

   * `static auto Invalid() noexcept -> Type` which returns `-1`; and
   * `static void Close(Type value) noexcept` which closes the file handle.

2. Alias the `UniqueHandle` with

   `using FileHandle = UniqueHandle<FileHandleTraits>;`

Then the usage of everywhere that takes a file handle as in input or an output
is replaced with `FileHandle`, that can use simple operations to know if the
handle is valid or not.

All methods in `rjcp::os:;qnx::native::file` use the `FileHandle` instead of an
`int` for safety. For interest, this would also abstract Operating System
differences between Windows and Unix platforms that have different
interpretations of an `int` and a `HANDLE` between 32-bit and 64-bit programming
models (although for this project, this fact is not relevant, but indicates the
potential robustness of this technique to avoiding some types of failures with
code reuse).

Should the handle go out of scope, the resource is automatically freed as per
the trait.

## 3. Abstracting Operating System Types

The C++ specifications use the type `std::size_t` which is usually an `unsigned
long` type. Under Posix, some function calls, such as `read` return a type of
`ssize_t` which C++ does not have a direct equivalent. This makes the `ssize_t`
a system specific type and should not be exposed to application layers.

The OS Wrapper layer should abstract away these types to provide only C++ types.
For example, the `read` function in the OS Wrapper layer has the signature:

```c++
auto read(FileHandle& fd, std::vector<std::uint8_t>& buf, std::size_t count) noexcept -> expected<std::size_t>;
```

## 4. The C++23 std::expected type

In the previous example of `read`, converting the type `ssize_t` to a
`std::size_t` could lose information. The C++23 construct `std::expected` is
[reimplemented](https://github.com/TartanLlama/expected/blob/96d547c03d2feab8db64c53c3744a9b4a7c8f2c5/include/tl/expected.hpp)
(CC0 license) that overcomes a problem of Systems Programming, where the return
code often represents the value of the function, unless it is `-1`, in which
case it is an error described by `errno`.

As a Systems Programmer, it is easy to ignore such result codes, leading to
cascaded effects.

By returning a `std::expected`, an entire class of errors for ignoring result
codes can be avoided. Every wrapped function can:

* Check the result is valid or an error
* If valid, return the correct result type casted to a C++ type, like
  `std::size_t`
* If invalid, retrieve the error code and present the error code to the caller.

The implementation of `std::expected` will automatically raise an exception
should an unexpected value try to be dereferenced, avoiding the use of invalid
results.

## 5. Emulating Layers

There is no API that implements emulation at this point in time. But should one
OS provide a method (e.g. a GNU extension), and another OS not provide this, it
could be possible to implement a specific implementation for the OS with missing
details. The CMake system could check for the missing implementation and add a
new C++ wrapper as a new file for that Operating System as needed.
