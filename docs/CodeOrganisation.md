# 1. Organisation of Code <!-- omit in toc -->

- [1. Namespaces Organisation](#1-namespaces-organisation)
- [2. Folder Organisation](#2-folder-organisation)
- [3. Include Files](#3-include-files)
- [4. Scope of each Source File](#4-scope-of-each-source-file)


## 1. Namespaces Organisation

The namespace hierarchy:

* rjcp::cpuid

  The main application that implements the resource manager API.

* rjcp::cpuid::tree

  Classes and methods that manage the in memory representation of all CPU
  objects, and the results of the CPUID instruction calls.

  Contains also the methods that can write the `CpuIdTree` to a `std::ostream`
  in XML format (e.g. a file, a memory buffer, or `std::cout` as an example).

* rjcp::qnx::os

  Contains C++ abstractions of the QNX Operating System. This layer represents
  the QNX OS projected to C++17 for implementing a simple, single threaded,
  resource manager. It is not intended however to be a reusable piece of code in
  other projects on its own.

* rjcp::qnx::os::native

  Code under this namespace is used to wrap around the QNX OS. It is where most
  of the system programming is provided, with usage of QNX types wrapped for
  higher levels of software. This would be taking opaque types and wrapping them
  in handles, and providing the wrapper functions to the QNX OS itself.

  In principle, this should be the only layer to replace when testing on non-QNX
  operating systems.

  Only the `rjcp::qnx::os` namespace should include headers from this namespace.

* rjcp::qnx::os::native::file

  A `unique_handle` that represents a file handle. Projections of Posix API that
  manipulate files, as free functions.

## 2. Folder Organisation

The upper level folders are:

* `cmake/modules/` - CMake modules.
  * `sanitizers/` - Open Source Linux Sanitizers
* `docs/` - Where documentation is kept (design, and other non-introductory
  documentation).
* `src/` - where the sources are kept that are compiled for the target resource
  manager.
  * Under this folder sources are organized in the same structure as the
    namespaces.
  * `CMakeLists.txt` - Build file for a static library (used for testing) and
    the main program.
  * `lib` - All testable code is put here. This project is small enough that it
    does not warrant having multiple libraries. It is used by the resource
    manager, tools and test cases.
    * Code in this folder matches the namespace organisation, so that
      `rjcp::os::qnx::native` would be in the folder `lib/os/qnx/native`.
  * `devc-cpuid` - The resource manager.
  * `cpuid-tool` - The tool to do integration tests on both Linux and QNX. It
    can be used to exercise the APIs and compare results, as well as extract
    information, which is useful for my other project,
    [RJCP.DLL.CpuId](https://github.com/jcurl/RJCP.DLL.CpuId/).
* `test/` - Implementation of test cases using google test.
  * The organisation of folders follow the same structure as the namespaces for
    the classes under test.
  * `CMakeLists.txt` - Build file for the google unit tests, links to the static
    library built under `src`.
  * `lib` - Mirrors the `src/lib` with test cases and test specific code.
* `toolchain/` - Files for toolchain builds.
  * `qcc710_x86_64` - Example toolchain file for QNX 7.1.0 x86_64 architecture.
  * `clang` - Example toolchain to use the clang compiler
* `CMakeLists.txt` - root project build file.
* `BUILD.md` - Quick instructions on building the repository using CMake. See
  your CMake documentation for more details.
* `LICENSE.md` - The license for this project.
* `README.md` - Introduction to this repository.
* `build.sh` - An example build script.

## 3. Include Files

This project is completely self contained, and it is not intended that code
would be exported as its own package. As such, all includes are available,
considered public as needed within the project. This puts the (small) burden on
the maintainer to understand the structure of the code when using new headers,
that the include makes sense.

## 4. Scope of each Source File

Each source file generally only contains one class. This leads to an explosion
of many files, but with the benefit of being able to quickly identify which file
contains what classes or headers.

