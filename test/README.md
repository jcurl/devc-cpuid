# Test Suites

This directory contains the google-test test suites, written in C++.

To run the tests, build the software from the root directory:

```sh
mkdir build && cd build
cmake .. -DENABLE_TEST=on -DADDRESS-SANITIZE=on -DCMAKE_BUILD_TYPE=Debug
make && ctest
```

To run the test cases on a remote target, copy the files listed to the target
and execute the binary directly there:

* `./test/lib/devc-cpuid-test`

## Directory Structure

The source files for the test cases mirror the same directory structure as the
sources themselves, and end with `_test.cpp`.

Most of the logic is in the static library, and linked in where necessary.
Therefore, most of the tests are in the `lib` directory.

## Test and Target Strategy

Test cases are written to run on a Linux host, as well as a QNX host. When run
under Linux, only POSIX functionality can be tested. Functionality that is
specific to QNX can only run on a QNX target.

Rationale for this is to allow a faster write, compile and test cycle where
possible.

## Manually Running Coverage

If you've successfully used the option with code coverage, you can generate
coverage for a specific test case as:

```sh
export BASEDIR=/home/jcurl/source/devc-cpuid
export BUILDDIR=${BASEDIR}/build_lin

cd ${BUILDDIR}/test/lib
lcov --directory ${BUILDDIR} --zerocounters
./devc-cpuid-test --gtest_filter=*Move*
mkdir ccov
lcov --directory ${BUILDDIR}/ --base-directory ${BASEDIR}/ --no-external --capture --output-file ./ccov/test.info
genhtml -o ./ccov ./ccov/test.info
```
