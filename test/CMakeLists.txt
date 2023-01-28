add_executable(sta main.cpp)

target_link_libraries(sta libsta)

target_include_directories(sta
    PUBLIC ${STA_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${STA_BINARY_DIR}/bin)
