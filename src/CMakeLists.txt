file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR} "pd/*.cpp" "pd/core/frame/DataFrame/*.cpp" "pd/core/series/Series/*.cpp" "pd/core/internal/httpreader/*.cpp")

add_library(${PD} ${SOURCES})

target_include_directories(${PD} PUBLIC ${np_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include)
