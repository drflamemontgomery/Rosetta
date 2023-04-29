#ifndef __ROSETTA_HID_DEV
#define __ROSETTA_HID_DEV

#define MAX_HID_OUT 4
#define OUT_REPORT_SIZE 8

#define MAX_INPUT_DEVICES 8
#define MAX_ATTACHED_DEVICES 8

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pio_usb.h"

// Report of the faceoff pro controller
typedef struct faceoff_pro_controller_data {
  uint16_t buttons; // max value 0b0011111111111111 or 0x3fff
  uint8_t hat;      // works like a compass 0x00 == Top, 0x07 == Top Left, 0x0f == Center 
  uint8_t axis_x;
  uint8_t axis_y;
  uint8_t axis_z;
  uint8_t axis_rz;
  uint8_t __unused null_byte; // byte that stays zero
} pro_controller_data;

typedef struct struct_xbox_controller {
  uint16_t buttons;
  uint8_t lt;
  uint8_t rt;
  uint8_t axis_x;
  uint8_t axis_y;
  uint8_t axis_z;
  uint8_t axis_rz;

  uint8_t rumble_left;
  uint8_t rumble_right;

} xbox_controller;

typedef struct struct_ps1_racing_wheel {
  uint16_t buttons;
  uint8_t axis_x;
  uint8_t axis_y;
  uint8_t paddle;
} ps1_racing_wheel;

extern pro_controller_data hid_device_out[MAX_HID_OUT];

typedef enum enum_byte_config_type {
  CH_BUTTON_PASS_THROUGH = 0x00,
  CH_HAT_PASS_THROUGH    = 0x10,
  CH_AXIS_PASS_THROUGH   = 0x20,

  CH_BUTTON = 0x01,
  CH_HAT    = 0x11,
  CH_AXIS   = 0x21,

  CH_USER_DEFINED = 0xFF,
} byte_config_type;


typedef struct struct_config_header {
  uint8_t size;
  uint8_t controller_id;
  uint8_t start_byte;
  uint8_t byte_size;
  uint8_t active_type;
  uint8_t mapping_type;

  // active_data
  // mapping_data
} config_header;

typedef struct struct_config_holder {
  uint8_t num_of_configs;
  config_header** configs;  
} config_holder;

typedef struct struct_device {
  bool connected;
  uint8_t data_len;
  uint8_t *data;
  config_holder new_configs;
} device_t;

typedef struct struct_input_dev {
  usb_device_t *_device;
  int num_of_devices;
  int* devices_idx;
  uint8_t driver_idx;
} input_dev_t;

void run_config_holder(input_dev_t* dev);

typedef struct struct_usb_driver {
  bool (*is_driver_for_device)(uint16_t, uint16_t); // is_driver_for_device(int vid, int pid);
  void (*initialize_device)(input_dev_t*); // initialize_device(input_dev_t* device);
  void (*get_data_for_device)(input_dev_t*); // get_data_for_device(input_dev_t* device);
} usb_driver_t;

extern device_t attached_devices[MAX_ATTACHED_DEVICES]; 

#endif

