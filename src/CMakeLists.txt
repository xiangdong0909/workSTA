set(LIBRARY_OUTPUT_PATH ${STA_BINARY_DIR}/lib)

add_library(libsta sta.cpp)

target_include_directories(libsta
    PUBLIC ${STA_SOURCE_DIR}/include)

set_target_properties(libsta PROPERTIES OUTPUT_NAME "sta")
