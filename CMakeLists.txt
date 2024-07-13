cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Rosetta)

pico_sdk_init()

if(NOT DEFINED PICO_PIO_USB_DIR)
  set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB")
endif()
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

message(STATUS "pio_dir ${PICO_PIO_USB_DIR}")

set(PROJECT_SRCS
  src/main.c
  src/usb_descriptors.c
  src/setup.c
  src/usb_device.c
  src/usb_host.c
  src/drivers.c

  src/led.c
  src/fs.c
  src/config.c

  src/ssd1306.c
  src/text.c
  src/screen.c

  src/temp_configs.c

 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/dcd_pio_usb.c
 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c
)

message(STATUS "srcs: ${PROJECT_SRCS}")

add_executable(Rosetta
  ${PROJECT_SRCS}
)

target_compile_definitions(Rosetta PRIVATE PIO_USB_USE_TINYUSB)
target_include_directories(Rosetta PRIVATE src)

target_link_libraries(Rosetta
	pico_stdlib pico_pio_usb tinyusb_device tinyusb_host hardware_flash)

pico_add_extra_outputs(Rosetta)
