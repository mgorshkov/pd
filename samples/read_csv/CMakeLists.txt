cmake_minimum_required(VERSION 3.18.0)

set(READ_CSV read_csv)

project(${READ_CSV})

# C++ 20
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    np
    GIT_REPOSITORY https://github.com/mgorshkov/np.git
    GIT_TAG main
)

FetchContent_MakeAvailable(np)

find_library(PD_LIBRARY
    NAMES pd libpd
    PATHS
        ../../build/src
        /usr/local/lib
        /usr/lib
    NO_DEFAULT_PATH
)
if(NOT PD_LIBRARY)
    message(FATAL_ERROR "PD library not found! Build parent pd first.")
endif()

message(STATUS "Found PD: ${PD_LIBRARY}")

include_directories(../../include ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include)

add_executable(${READ_CSV})

target_sources(${READ_CSV} PUBLIC main.cpp)

set (LIBS ${PD_LIBRARY}
    ${PTHREAD}
    ssl
    crypto
    fmt)

target_link_libraries(
    ${READ_CSV}
    ${LIBS}
)

install(
    TARGETS ${READ_CSV}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${READ_CSV}
)
