#ifndef __ROSETTA_HID_DEV
#define __ROSETTA_HID_DEV

#define MAX_HID_OUT 4
#define OUT_REPORT_SIZE 8

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pio_usb.h"

typedef struct faceoff_pro_controller_data {
  uint16_t buttons;
  uint8_t hat; 
  uint8_t axis_x;
  uint8_t axis_y;
  uint8_t axis_z;
  uint8_t axis_rz;
  uint8_t __unused_byte;
} pro_controller_data;

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
  uint8_t __unused;
} hat_config;

struct struct_config_elem;

typedef struct struct_user_config {
  void (*run_config)(struct struct_config_elem);
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

typedef struct struct_hid_dev {
  usb_device_t *_device;
  uint8_t data_len;
  uint8_t *data;
  config_t config;  
  uint8_t driver_idx;
} hid_dev_t;

void run_config(hid_dev_t* dev);

typedef struct struct_usb_driver {
  bool (*is_driver_for_device)(uint16_t, uint16_t); // is_driver_for_device(int vid, int pid);
  void (*initialize_device)(hid_dev_t*, int); // initialize_device(hid_dev_t* device, int dev_id);
  void (*get_data_for_device)(hid_dev_t*); // get_data_for_device(hid_dev_t* device);
} usb_driver_t;
#endif

