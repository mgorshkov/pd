cmake_minimum_required(VERSION 3.13.0)

set(PROJECT read_csv)

project(${PROJECT})

if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
endif()

include_directories(../../include ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include)

add_executable(${PROJECT})

target_sources(${PROJECT} PUBLIC main.cpp)

target_link_libraries(
    ${PROJECT}
    ${PTHREAD}
    pd
    ssl)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT}
)
