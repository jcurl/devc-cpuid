# Design <!-- omit in toc -->

This document presents the design and structure of the `devc-cpuid` resource
manager.

Motivation of this project is not to just implement a CPUID driver that mimics
Linux, but to:

* Design it in a way using a modern C++17 compiler (whereby normally QNX
  resource managers are typically written in C);
* Builds should be done using the [CMake](https://cmake.org/) build system,
  instead of the Momentics IDE;
* Use google test for unit testing of the resource manager, allowing unit
  testing also on a non-QNX host;
* Investigate how to obtain code coverage when testing;
* Extend non-QNX hosts to also use sanitizers for spotting possible errors.

It is also my first C++ project of note, where I've mostly written C and C# in
the past.

- [1. Language and Code Structure](#1-language-and-code-structure)
  - [1.1. Namespaces Organisation](#11-namespaces-organisation)
  - [1.2. Folder Organisation](#12-folder-organisation)

## 1. Language and Code Structure

### 1.1. Namespaces Organisation

The namespace hierarchy:

* rjcp::qnx::cpuid

  The main application that implements the resource manager API.

* rjcp::qnx::os

  Contains C++ abstractions of the QNX Operating System. This layer represents
  the QNX OS prjected to C++17 for implementing a simple, single threaded,
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

### 1.2. Folder Organisation

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
* `test/` - Implementation of test cases using google test.
  * The organisation of folders follow the same structure as the namespaces for
    the classes under test.
  * `CMakeLists.txt` - Build file for the google unit tests, links to the static
    library built under `src`.
* `toolchain/` - Files for toolchain builds.
  * `qcc710_x86_64` - Example toolchain file for QNX 7.1.0 x86_64 architecture.
  * `clang` - Example toolchain to use the clang compiler
* `CMakeLists.txt` - root project build file.
* `BUILD.md` - Quick instructions on building the repository using CMake. See
  your CMake documentation for more details.
* `LICENSE.md` - The license for this project.
* `README.md` - Introduction to this repository.
* `build.sh` - An example build script.
