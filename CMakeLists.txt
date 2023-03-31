cmake_minimum_required(VERSION 3.13.0)

set(PD pd)

project(${PD} VERSION 0.0.1 DESCRIPTION "Pandas library methods on top of NP library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

FetchContent_Declare(
    np
    GIT_REPOSITORY https://github.com/mgorshkov/np.git
    GIT_TAG main
)

FetchContent_MakeAvailable(np)

FetchContent_Declare(
    openssl
    GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
)

FetchContent_MakeAvailable(openssl)

add_library(OpenSSL::SSL ALIAS ssl)
add_library(OpenSSL::Crypto ALIAS crypto)

add_subdirectory(src)

# Make unit tests and doc if it's the main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(MAKE_UNIT_TESTS ON)
    set(MAKE_DOC ON)
    set(MAKE_SAMPLE ON)
else()
    set(MAKE_UNIT_TESTS OFF)
    set(MAKE_DOC OFF)
    set(MAKE_SAMPLE OFF)
endif()

if (MAKE_UNIT_TESTS)
    message(STATUS "Enable testing in ${PD}")
    add_subdirectory(unit_tests)
    include (CTest)
    enable_testing()
endif()

if (MAKE_DOC)
    message(STATUS "Enable doxygen in ${PD}")
    add_subdirectory(doc)
endif()

if (MAKE_SAMPLE)
    message(STATUS "Enable samples build in ${PD}")
    add_subdirectory(samples/read_csv)
endif()

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/pd
    DESTINATION include
    COMPONENT ${PD_UNIT_TESTS}
)
