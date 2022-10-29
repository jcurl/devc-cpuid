# Building <!-- omit in toc -->

This project requires the installation of:

* A C++17 compiler (Ubuntu 20.04, Ubuntu 22.04, QNX SDP 7.1.0).  The QNX 7.1.0
  SDP should be installed and the environment set up (see the QNX documentation
  on doing this). Ensure to have sanitizers installed for this feature.
* CMake 3.14 or later (so Ubuntu 20.04 LTS or later should be used).
* Clang-Tidy should be installed for additional diagnostics when building on the
  Linux host
* An Internet connection is required for downloading GoogleTest during the
  configuration stage.

- [1. Tested Environments](#1-tested-environments)
- [2. Building and Executing Tests](#2-building-and-executing-tests)
  - [2.1. Building on the Command Line](#21-building-on-the-command-line)
    - [2.1.1. Convenience Build Script](#211-convenience-build-script)
    - [2.1.2. Configure with CMake](#212-configure-with-cmake)
    - [2.1.3. Building the Software](#213-building-the-software)
  - [2.2. Build Features](#22-build-features)
    - [2.2.1. Providing a DEBUG/RELEASE build](#221-providing-a-debugrelease-build)
    - [2.2.2. Selecting the Compiler](#222-selecting-the-compiler)
    - [2.2.3. Providing Compiler Flags](#223-providing-compiler-flags)
    - [2.2.4. Disabling clang-tidy](#224-disabling-clang-tidy)
    - [2.2.5. Enabling Sanitizers](#225-enabling-sanitizers)
  - [2.3. Test Suites](#23-test-suites)
    - [2.3.1. Running Tests](#231-running-tests)

## 1. Tested Environments

The build has been tested on Ubuntu 22.04 with GCC 11, Clang 14.0 and QCC 8.3.0
(QNX 7.1.0 SDP).

## 2. Building and Executing Tests

A lot of the information here is part of the `cmake` documentation, but is
listed here for convenience.

### 2.1. Building on the Command Line

#### 2.1.1. Convenience Build Script

A script is provided for convenience to build for different environments.

* Linux Native Host
  * Debug with Clang-Tidy and Sanitizers for GCC and Clang
  * Release
  * Google Tests
  * Code Coverage for GCC/LCOV and Clang
* QNX SDP 7.1.0
  * Release Mode
  * Google Tests

To build the project on Unix like shells, build in a VPATH environment:

```sh
. ~/qnx/qnx710/qnxsdp-env.sh
./build.sh
```

See the file `build.sh` for the build invocations.

#### 2.1.2. Configure with CMake

Of course, the build script is not required to build the software. It should
still be simple to build on the command line using `cmake` direct.

Building the Linux native host binaries:

```sh
mkdir build_lin && cd build_lin
cmake ..
```

Building the resource manager for QNX 7.1.0 SDP

```sh
. ~/qnx/qnx710/qnxsdp-env.sh
mkdir build_qnx && cd build_qnx
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain/qcc710_x86_64
```

#### 2.1.3. Building the Software

Once you've configured the software with `cmake`, build it with your make
command. E.g. on Unix hosts:

```sh
make
```

If you want verbose information while building:

```sh
VERBOSE=1 make
```

### 2.2. Build Features

#### 2.2.1. Providing a DEBUG/RELEASE build

For the Debug build:

```sh
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

And similarly for a release build:

```sh
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 2.2.2. Selecting the Compiler

To compile using common compilers on your local machine (Linux):

| Compiler | Command                        | Alternative                                          |
| -------- | ------------------------------ | ---------------------------------------------------- |
| GCC      | `CXX=g++ C=gcc cmake ..`       | `cmake ..`                                           |
| Clang    | `CXX=clang++ C=clang cmake ..` | `cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain/clang` |

For cross compiling, provide a toolchain file, e.g.

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain/qcc710_x86_64
```

#### 2.2.3. Providing Compiler Flags

To override with your own compiler flags

```sh
cmake -DCMAKE_CXX_FLAGS="-Wunused-parameter" ..
```

or equivalently with

```sh
cmake -E env CXXFLAGS="-Wunused-parameter" cmake ..
```

#### 2.2.4. Disabling clang-tidy

By default, CMake will look for and use clang-tidy. If you don't want this, then
build with it disabled. If clang-tidy is not installed or not available, it is
ignored.

```sh
cmake .. -DENABLE_CLANG_TIDY=off
```

#### 2.2.5. Enabling Sanitizers

Sanitizers are provided by
[arsenm/sanitizers-cmake](cmake/modules/sanitizers/VERSION.md).

To build with sanitizers:

```sh
cmake .. -DSANITIZE_ADDRESS=on
VERBOSE=1 make
```

### 2.3. Test Suites

#### 2.3.1. Running Tests

After building on the host, you can run the tests on the host (from the build
directory) with:

```sh
make
ctest
```

Running `ctest` of course only works for test suites on the host. Cross compile
environments are different, the executable binary must first be copied across
and be executed directly on the target.

If you want to run the google test suite directly

```sh
cmake
./test/lib/devc-cpuid-test
```
