# QNX CPUID Resource Manager

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

## Further Reading

General Information

* [MIT License](../LICENSE.md)
* [Building with CMake)](../BUILD.md)
* [Organisation of Code](CodeOrganisation.md). Describes how the repository is
  laid out, and some principles for structure in this repository.

Library Design Information

* [CPUID Classes](CpuIdDesign.md). Describes the classes to create a `ICpuId`
  object via factories derived from `ICpuIdFactory`.
* [C++ OS Wrappers](OsWrapper.md). Wrapping POSIX and OS calls to present a
  consistent C++ interface.

CPUID Tool Design

* [CPUID Tool Design](Tool-CpuIdDesign.md). Describes the design of the simple
  tool, used to dump the CPUID instruction information.
