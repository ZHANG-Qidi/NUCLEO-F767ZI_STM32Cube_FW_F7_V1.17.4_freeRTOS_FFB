file(GLOB_RECURSE HID_FFB_SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(hid_ffb STATIC
    ${HID_FFB_SRC}
)

target_include_directories(hid_ffb PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(hid_ffb PUBLIC
    arduino_core
    arduino_foc
    freertos_kernel
    tinyusb
)
