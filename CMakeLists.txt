cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Rosetta)

pico_sdk_init()

set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB/")
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

add_executable(Rosetta
	src/main.c
  src/usb_descriptors.c
  src/drivers.c
  src/config.c
  src/configs.c
)

target_include_directories(Rosetta PRIVATE src)

target_link_libraries(Rosetta
	pico_stdlib pico_pio_usb tinyusb_device)

pico_add_extra_outputs(Rosetta)
