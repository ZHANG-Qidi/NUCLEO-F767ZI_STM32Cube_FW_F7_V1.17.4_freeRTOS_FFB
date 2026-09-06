cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE TINYUSB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

add_library(tinyusb STATIC ${TINYUSB_SOURCES})

target_include_directories(tinyusb PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/hw
)

target_link_libraries(tinyusb PUBLIC
    stm32cubemx
)
