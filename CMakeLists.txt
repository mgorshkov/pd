cmake_minimum_required(VERSION 3.18.0)

# Project name
set(PROJECT_NAME pd)

# Set up project
project(${PROJECT_NAME} VERSION 0.0.7 DESCRIPTION "⚡ Data manipulation and analysis library in C++ | CUDA GPU + (AVX2/AVX512/AMX) CPU" LANGUAGES CXX)

# Tune up various optimizations
# Enable CUDA support
option(ENABLE_CUDA "Enable CUDA optimizations" ON)
# Set up options for basic compile optimizations
option(ENABLE_AVX2 "Enable AVX2 optimizations" ON)
option(ENABLE_AVX512 "Enable AVX512 optimizations" OFF)
option(ENABLE_AMX "Enable AMX (Advanced Matrix Extensions) optimizations" OFF)
# Set up options for OpenMP support
option(ENABLE_OPENMP "Enable OpenMP" ON)

include(FetchContent)

FetchContent_Declare(
    cmake
    GIT_REPOSITORY https://github.com/mgorshkov/cmake.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cmake)

# Apply global cmake options
include(${cmake_SOURCE_DIR}/Config.cmake)

FetchContent_Declare(
    np
    GIT_REPOSITORY https://github.com/mgorshkov/np.git
    GIT_TAG main
    CMAKE_ARGS
    -DENABLE_CUDA=${ENABLE_CUDA}
    -DENABLE_OPENMP=${ENABLE_OPENMP}
    -DENABLE_SSE2=${ENABLE_SSE2}
    -DENABLE_SSE3=${ENABLE_SSE3}
    -DENABLE_AVX=${ENABLE_AVX}
    -DENABLE_AVX2=${ENABLE_AVX2}
    -DENABLE_AVX512=${ENABLE_AVX512}
    -DENABLE_AMX=${ENABLE_AMX}
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DDEBUG_INFO=${DEBUG_INFO}
)
FetchContent_MakeAvailable(np)

set(OPENSSL ON)

if (OPENSSL)
    FetchContent_Declare(
        openssl
        GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
    )

    FetchContent_MakeAvailable(openssl)

    add_library(OpenSSL::SSL ALIAS ssl)
    add_library(OpenSSL::Crypto ALIAS crypto)
    # Suppress unused parameter warnings in OpenSSL
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(ssl PRIVATE -Wno-unused-parameter -Wno-missing-field-initializers)
        target_compile_options(crypto PRIVATE -Wno-unused-parameter -Wno-missing-field-initializers)
        if(TARGET openssl)
            target_compile_options(openssl PRIVATE -Wno-unused-parameter -Wno-missing-field-initializers)
        endif()
    endif()

    add_compile_definitions(OPENSSL)
endif()

add_subdirectory(src)

set(LIBS ${LIBS} np OpenSSL::SSL OpenSSL::Crypto)

# Apply target-dependent cmake options
include(${cmake_SOURCE_DIR}/Target.cmake)

set(MAKE_DOC OFF)

# Make unit tests and sample if it's the main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(MAKE_UNIT_TESTS ON)
else()
    set(MAKE_UNIT_TESTS OFF)
endif()

if (MAKE_UNIT_TESTS)
    message(STATUS "Enable testing in ${PROJECT_NAME}")
    add_subdirectory(unit_tests)
    include (CTest)
    enable_testing()
endif()

if (MAKE_DOC)
    message(STATUS "Enable doxygen in ${PROJECT_NAME}")
    add_subdirectory(doc)
endif()

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/PROJECT_NAME
    DESTINATION include
    COMPONENT ${PROJECT_NAME_UNIT_TESTS}
)
