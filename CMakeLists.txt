cmake_minimum_required(VERSION 3.14)
project(devc-cpuid C CXX)

option(ENABLE_CLANG_TIDY "Enable checks using Clang-Tidy if available" ON)
option(ENABLE_TEST "Enable building tests" ON)

# See https://github.com/arsenm/sanitizers-cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/sanitizers" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        message(STATUS "LLVM clang-tidy is being used with -DENABLE_CLANG_TIDY=on")
        # For info, see here: https://clang.llvm.org/extra/clang-tidy/
        set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}"
            "-checks=bugprone-*,\
                    modernize-*,\
                    cppcoreguidelines-*,\
                    -clang-diagnostic-unused-const-variable,\
                    -clang-diagnostic-c++17-extensions")
    else()
        message(STATUS "LLVM clang-tidy is enabled but NOT found")
    endif()
else()
    message(STATUS "LLVM clang-tidy is disabled with -DENABLE_CLANG_TIDY=off")
endif()

if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    if((CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang") OR
       (CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
       (CMAKE_CXX_COMPILER_ID MATCHES "QCC"))
        add_compile_options(-Wall -Wextra)
    endif()
endif()

# Checking if we should enable coverage. Only do so in DEBUG and if testing is enabled
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/coverage" ${CMAKE_MODULE_PATH})
include(code-coverage)
add_code_coverage()
if(CODE_COVERAGE)
    add_compile_options(-O0)
endif()

add_subdirectory(src)

if(ENABLE_TEST)
    message(STATUS "Test suite enabled with -DENABLE_TEST=on (execute with make && ctest)")
    include(FetchContent)  # CMake 3.14 or later

    # https://google.github.io/googletest/quickstart-cmake.html
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        GIT_SHALLOW 1
        GIT_PROGRESS 1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(test)
    enable_testing()
else()
    message(STATUS "Test suite disabled with -DENABLE_TEST=off")
endif()
