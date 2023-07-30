cmake_minimum_required(VERSION 3.13.0)

set(READ_CSV read_csv)

project(${READ_CSV})

include_directories(../../include ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include)

add_executable(${READ_CSV})

target_sources(${READ_CSV} PUBLIC main.cpp)

target_link_libraries(
    ${READ_CSV}
    ${PTHREAD}
    pd
    ssl)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${READ_CSV}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${READ_CSV}
)
