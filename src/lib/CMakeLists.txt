#set(BINARY devc-cpuid-lib)
#set(SOURCES
#    )

#add_library(${BINARY} STATIC ${SOURCES})
#target_compile_features(${BINARY} PUBLIC cxx_std_17)

#if(CLANG_TIDY_EXE)
#    set_target_properties(${BINARY} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
#endif()

#add_sanitizers(${BINARY})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
