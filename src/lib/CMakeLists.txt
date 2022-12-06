include(CheckSymbolExists)
include(CMakePushCheckState)

set(BINARY devc-cpuid-lib)
set(SOURCES
    cpuid/cpuid_default.cpp
    cpuid/cpuid_device.cpp
    cpuid/cpuid_factory.cpp
    cpuid/cpuid_native.cpp
    cpuid/cpuid_register.cpp
    cpuid/get_cpuid.cpp
    cpuid/tree/cpuid_processor.cpp
    cpuid/tree/cpuid_tree.cpp
    cpuid/tree/cpuid_write_xml.cpp
    os/qnx/native/file/file.cpp
)

find_package(Threads REQUIRED)

cmake_push_check_state(RESET)
list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
list(APPEND CMAKE_REQUIRED_LIBRARIES pthread)
check_symbol_exists(pthread_setaffinity_np pthread.h HAVE_PTHREAD_SETAFFINITY_NP)
cmake_reset_check_state()

# We need to be able to set the affinity of a thread to a particular CPU
if(HAVE_PTHREAD_SETAFFINITY_NP)
    add_compile_definitions(_GNU_SOURCE)
    set(SOURCES ${SOURCES}
        cpuid/cpuid_native_linux.cpp
    )
else()
    # QNX doesn't have `pthread_getaffinity_np`, so we need to use `ThreadCtl`.
    check_symbol_exists(ThreadCtl "sys/neutrino.h" HAVE_NTO_THREADCTL)
    if(HAVE_NTO_THREADCTL)
        set(SOURCES ${SOURCES}
            cpuid/cpuid_native_qnx.cpp
        )
    else()
        message(FATAL_ERROR "Can't find GNU Pthreads or NTO ThreadCtl()")
    endif()
endif()

add_library(${BINARY} STATIC ${SOURCES})
set_target_properties(${BINARY} PROPERTIES OUTPUT_NAME "devc-cpuid")  #because libdevc-cpuid-lib.a is weird
target_compile_features(${BINARY} PUBLIC cxx_std_17)

if(CLANG_TIDY_EXE)
    set_target_properties(${BINARY} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

add_sanitizers(${BINARY})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_code_coverage(${BINARY}
    EXCLUDE
    ${CMAKE_SOURCE_DIR}/src/lib/stdext/*
)
