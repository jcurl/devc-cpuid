include(GoogleTest)

set(BINARY devc-cpuid-test)
set(SOURCES
    cpuid/cpuid_default_test.cpp
    cpuid/cpuid_device_test.cpp
    cpuid/cpuid_native_test.cpp
    cpuid/cpuid_register_test.cpp
    os/qnx/native/file/file_test.cpp
    os/qnx/native/filehandle_type.c
    os/qnx/native/opaque_type.c
    os/qnx/native/unique_handle_test.cpp
)

add_executable(${BINARY} ${SOURCES})
target_compile_features(${BINARY} PUBLIC cxx_std_17)
target_link_libraries(${BINARY} PUBLIC devc-cpuid-lib GTest::gtest_main ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME ${BINARY} COMMAND ${BINARY})

if(CLANG_TIDY_EXE)
    set_target_properties(${BINARY} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

add_sanitizers(${BINARY})

target_code_coverage(${BINARY}
    EXCLUDE
    ${CMAKE_SOURCE_DIR}/src/lib/stdext/*
    ${CMAKE_SOURCE_DIR}/test/*
    ${CMAKE_BINARY_DIR}/_deps/*
)

# Access the source directories under test
include_directories(
    ${CMAKE_SOURCE_DIR}/src/lib
    ${CMAKE_SOURCE_DIR}/test/lib
)

enable_testing()
