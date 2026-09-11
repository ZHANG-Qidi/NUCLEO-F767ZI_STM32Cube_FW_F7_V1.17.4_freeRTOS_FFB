file(GLOB_RECURSE ARDUINO_CORE_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(arduino_core STATIC
    ${ARDUINO_CORE_SRC}
)

target_include_directories(arduino_core PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cores/arduino
    ${CMAKE_CURRENT_LIST_DIR}/libraries/Wire/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/SPI/src
)

target_link_libraries(arduino_core PRIVATE
    stm32cubemx
)
