file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR} "pd/*.cpp" "pd/core/frame/DataFrame/*.cpp" "pd/core/series/Series/*.cpp" "pd/core/internal/httpreader/*.cpp")

add_library(${PD} ${SOURCES})

target_include_directories(${PD} PUBLIC ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include)

if (MSVC)
    target_compile_options(${PD} PUBLIC /W4 /WX)
    target_compile_definitions(${PD} PUBLIC -DNOMINMAX) # allow using min/max in the project
    target_compile_definitions(${PD} PUBLIC -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PD} PUBLIC -Wall -Wextra -pedantic -Werror)
endif()
