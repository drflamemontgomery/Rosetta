cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pio_test)

pico_sdk_init()

set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB/")
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

add_executable(pio_test
	src/main.c
  src/usb_descriptors.c
)

target_include_directories(pio_test PRIVATE src)

target_link_libraries(pio_test
	pico_stdlib pico_pio_usb tinyusb_device)

#pico_enable_stdio_usb(pio_test 1)
#pico_enable_stdio_uart(pio_test 0)

pico_add_extra_outputs(pio_test)
