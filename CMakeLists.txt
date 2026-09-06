cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME NUCLEO-F767ZI_STM32Cube_FW_F7_V1.17.4_freeRTOS_FFB)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_subdirectory(ArduinoCore-avr/ArduinoCore-avr-1.8.7)
add_subdirectory(Arduino-FOC/Arduino-FOC-2.2)
add_subdirectory(TinyUSB/tinyusb-0.20.0)

add_library(freertos_headers INTERFACE)
target_include_directories(freertos_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/FreeRTOS-KernelV11.3.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/FreeRTOS-KernelV11.3.0/portable/GCC/ARM_CM7/r0p1
)

target_link_libraries(arduino_core PRIVATE freertos_headers)
target_link_libraries(tinyusb PRIVATE freertos_headers)

set(FREERTOS_PORT GCC_ARM_CM7)
set(FREERTOS_PORT GCC_ARM_CM7 CACHE STRING "" FORCE)

set(FREERTOS_HEAP 4 CACHE STRING "FreeRTOS heap implementation")

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM INTERFACE
    ${CMAKE_SOURCE_DIR}/Inc
)

add_subdirectory(FreeRTOS/FreeRTOS-KernelV11.3.0)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -u _printf_float)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/ffb_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/ffb_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/foc_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/foc_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/usb_setup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/usb_loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/wheel_registry.c

    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE

    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
    arduino_core
    arduino_foc
    freertos_kernel
    tinyusb
)
