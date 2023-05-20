cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Rosetta)

pico_sdk_init()

if(NOT DEFINED PICO_PIO_USB_DIR)
set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB")
endif()
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

message(STATUS "pio_dir ${PICO_PIO_USB_DIR}")

#file(GLOB_RECURSE PROJECT_SRCS "src/old/*.c")

#set(UNUSED_SRCS
set(PROJECT_SRCS
  src/main.c
  src/usb_descriptors.c
  src/setup.c
  src/usb_device.c
  src/usb_host.c
  src/drivers.c

  src/output.c

  src/led.c
)

message(STATUS "srcs: ${PROJECT_SRCS}")

add_executable(Rosetta
  ${PROJECT_SRCS}
)

target_include_directories(Rosetta PRIVATE src)

target_link_libraries(Rosetta
	pico_stdlib pico_pio_usb tinyusb_device hardware_flash)

pico_add_extra_outputs(Rosetta)
