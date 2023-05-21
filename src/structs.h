#pragma once

#include "pico/stdlib.h"
#include "pio_usb.h"

//========================================---
// Data Structures
//========================================---

typedef struct struct_output_controller {
  uint16_t buttons;
  uint8_t hat;
  int8_t axis_x;
  int8_t axis_y;
  int8_t axis_z;
  int8_t axis_rz;
  uint8_t __unused null_byte;
} output_controller_t;


//========================================---
// Config Structures
//========================================---

typedef enum enum_byte_config_type {
  CH_BUTTON_PASS_THROUGH = 0x00,
  CH_HAT_PASS_THROUGH    = 0x10,
  CH_AXIS_PASS_THROUGH   = 0x20,

  CH_BUTTON = 0x01,
  CH_HAT    = 0x11,
  CH_AXIS   = 0x21,

  CH_USER_DEFINED = 0xFF,
} byte_config_type_t;

typedef struct struct_config_header {
  uint8_t size;
  uint8_t controller_id;
  uint8_t start_byte;
  uint8_t byte_size;
  uint8_t active_type;
  uint8_t mapping_type;
} config_header_t;

typedef struct struct_config_holder {
  uint8_t num_of_configs;
  config_header_t **configs;
} config_holder_t;

//========================================---
// Input Device Structures
//========================================---

typedef struct struct_device {
  bool connected;
  uint8_t data_len;
  uint8_t *data;
  config_holder_t configs;
} device_t;

typedef struct struct_usb_input {
  usb_device_t *_device;
  int num_of_devices;
  int* devices_idx;
  uint8_t driver_idx;
} usb_input_t;

//========================================---
// Driver Structures
//========================================---

typedef struct struct_usb_driver {
  bool (*is_driver_for_device)(uint16_t, uint16_t); // (uint16_t vid, uint16_t pid)
  void (*initialize_device)(usb_input_t*); // (usb_input_t* usb_input_device)
  void (*get_data_for_device)(usb_input_t*); // (usb_input_t* usb_input_device)
} usb_driver_t;

#define USB_DRIVER(is_driver_for_device, initialize_device, get_data_for_device) {\
  is_driver_for_device,\
  initialize_device,\
  get_data_for_device\
  }

#define USB_DRIVER_STRUCT(NAME) USB_DRIVER(NAME##_is_driver_for_device, NAME##_initialize_device, NAME##_get_data_for_device)

#define IS_DRIVER(NAME) NAME##_is_driver_for_device
#define INIT_DRIVER(NAME) NAME##_initialize_device
#define GET_DATA_DRIVER(NAME) NAME##_get_data_for_device
