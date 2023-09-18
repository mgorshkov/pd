cmake_minimum_required(VERSION 3.13.0)

set(PD_UNIT_TESTS pd_unit_tests)

project(${PD_UNIT_TESTS} VERSION 0.0.4 DESCRIPTION "Pd Unit Tests")

add_definitions(-DTEST_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/test_data")

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(../include include ${googletest_SOURCE_DIR}/googletest/include ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include)

add_executable(${PD_UNIT_TESTS})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR} "src/*.cpp")
target_sources(${PD_UNIT_TESTS} PUBLIC ${SOURCES})

add_test(NAME ${PD_UNIT_TESTS} COMMAND tests)

set (LIBS
    pd
    gtest
    ${PTHREAD})

if (OPENSSL)
    set (LIBS ${LIBS} ssl)
endif ()

target_link_libraries(${PD_UNIT_TESTS} ${LIBS})

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PD_UNIT_TESTS}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PD_UNIT_TESTS}
)