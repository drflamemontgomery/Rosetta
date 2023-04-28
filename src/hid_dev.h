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

typedef enum enum_config_type {
  BUTTON,
  AXIS,
  HAT,
  USER_DEFINED,
} config_type;

typedef struct struct_button_config {
  uint8_t mask_from;
  uint16_t mask_to;
} button_config;

typedef struct struct_axis_config {
  uint8_t axis;
  uint8_t reversed;
} axis_config;

typedef struct struct_hat_config {
  uint8_t __unused unused_data;
} hat_config;

struct struct_config_elem;

typedef struct struct_user_config {
  void (*run_config)(struct struct_config_elem*, uint8_t* data);
  void* data;
} user_config;

typedef struct struct_config_elem {
  config_type type;
  uint8_t output; // Controller Output (0-3)
  uint8_t byte;
  union {
    button_config btn;
    axis_config axis;
    hat_config hat;
    user_config user;
  };
} config_elem;

typedef struct struct_config {
  uint8_t num_of_elems;
  config_elem* elems;
} config_t;

typedef struct struct_device {
  bool connected;
  uint8_t data_len;
  uint8_t *data;
  config_t *config;
} device_t;

typedef struct struct_input_dev {
  usb_device_t *_device;
  int num_of_devices;
  int* devices_idx;
  uint8_t driver_idx;
} input_dev_t;

void run_config(input_dev_t* dev);

typedef struct struct_usb_driver {
  bool (*is_driver_for_device)(uint16_t, uint16_t); // is_driver_for_device(int vid, int pid);
  void (*initialize_device)(input_dev_t*); // initialize_device(input_dev_t* device);
  void (*get_data_for_device)(input_dev_t*); // get_data_for_device(input_dev_t* device);
} usb_driver_t;

extern device_t attached_devices[MAX_ATTACHED_DEVICES]; 

#endif

