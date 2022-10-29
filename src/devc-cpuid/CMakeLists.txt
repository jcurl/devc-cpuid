set(BINARY ${CMAKE_PROJECT_NAME})
#set(SOURCES
#    )

add_executable(${BINARY} main.cpp)
target_compile_features(${BINARY} PUBLIC cxx_std_17)
#target_link_libraries(${BINARY} devc-cpuid-lib)

if(CLANG_TIDY_EXE)
    set_target_properties(${BINARY} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

add_sanitizers(${BINARY})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
