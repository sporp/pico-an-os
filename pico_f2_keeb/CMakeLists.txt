cmake_minimum_required(VERSION 3.13)
include(cmake/pico_sdk_import.cmake)

project(pico_f2_keyboard C CXX ASM)
pico_sdk_init()

add_executable(pico_f2_keyboard main.c)

target_include_directories(pico_f2_keyboard PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(pico_f2_keyboard PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pico_f2_keyboard pico_stdlib tinyusb_device tinyusb_board)

pico_add_extra_outputs(pico_f2_keyboard)
