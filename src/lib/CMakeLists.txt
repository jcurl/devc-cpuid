include(CheckSymbolExists)

set(BINARY devc-cpuid-lib)
set(SOURCES
    cpuid/cpuid_default.cpp
    cpuid/cpuid_native.cpp
    cpuid/cpuid_register.cpp
)

# QNX doesn't have `pthread_getaffinity_np`, so we need to use `ThreadCtl`.
check_symbol_exists(ThreadCtl "sys/neutrino.h" HAVE_NTO_THREADCTL)
if(NOT HAVE_NTO_THREADCTL)
    set(SOURCES ${SOURCES}
        cpuid/cpuid_native_linux.cpp
    )
else()
    set(SOURCES ${SOURCES}
        cpuid/cpuid_native_qnx.cpp
    )
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
