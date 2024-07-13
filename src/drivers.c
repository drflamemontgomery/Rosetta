#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "structs.h"
#include "tusb.h"

extern output_controller_t hid_output[MAX_OUTPUTS];
extern usb_input_t usb_devices[MAX_USB_INPUTS];
extern const uint8_t num_of_input_devices;
extern const usb_driver_t drivers[];

extern void run_config_holder(usb_input_t *usb_input);

void update_usb_drivers(usb_device_t *usb_device) {
  if (usb_device == NULL)
    return;

  for (int i = 0; i < MAX_OUTPUTS; i++) {
    static const output_controller_t default_data = {0x0000, 0x0f, 0x00, 0x00,
                                                     0x00,   0x00, 0x00};
    memcpy(&hid_output[i], &default_data, sizeof(default_data));
  }

  uint8_t found_devices = 0;
  for (int i = 0; i < MAX_USB_INPUTS; i++) {
    if (found_devices >= num_of_input_devices)
      break;

    found_devices++;
    usb_input_t *usb_input = &usb_devices[i];

    if (!usb_input->connected)
      continue;

    drivers[usb_input->driver_idx].get_data_for_device(usb_input);

    run_config_holder(usb_input);
  }
}

/*#define DEFAULT_DRIVER {\
  .is_driver_for_device = NULL,\
  .initialize_device    = default_driver_initialize_device,\
  .get_data_for_device  = default_driver_get_data_for_device,\
}*/

void INIT_DRIVER(default_driver)(usb_input_t *usb_input);
void GET_DATA_DRIVER(default_driver)(usb_input_t *usb_input);
#define DEFAULT_DRIVER                                                         \
  USB_DRIVER(NULL, INIT_DRIVER(default_driver), NULL,                          \
             GET_DATA_DRIVER(default_driver))

bool IS_DRIVER(faceoff)(uint16_t vid, uint16_t pid);
void INIT_DRIVER(faceoff)(usb_input_t *usb_input);
void DEINIT_DRIVER(faceoff)(usb_input_t *usb_input);
void GET_DATA_DRIVER(faceoff)(usb_input_t *usb_input);

bool IS_DRIVER(keyboard)(uint16_t vid, uint16_t pid);
void INIT_DRIVER(keyboard)(usb_input_t *usb_input);
void DEINIT_DRIVER(keyboard)(usb_input_t *usb_input);
void GET_DATA_DRIVER(keyboard)(usb_input_t *usb_input);

const usb_driver_t __in_flash("drivers") drivers[] = {
    DEFAULT_DRIVER,
    USB_DRIVER_STRUCT(faceoff),
    USB_DRIVER_STRUCT(keyboard),
};
// const uint8_t __in_flash("drivers") num_of_drivers =
// sizeof(drivers)/sizeof(usb_driver_t);
const uint8_t __in_flash("drivers") num_of_drivers = 3;

void deinitialize_device(usb_input_t *usb_input) {
  usb_input->connected = false;
  usb_input->data_len = 0;
  if (usb_input->data != NULL) {
    free(usb_input->data);
    usb_input->data = NULL;
  }
}

void attach_driver(usb_input_t *usb_input) {
  for (int i = 1; i < num_of_drivers; i++) {
    uint16_t vid, pid;
    tuh_vid_pid_get(usb_input->addr, &vid, &pid);
    if (!drivers[i].is_driver_for_device(vid, pid))
      continue;
    usb_input->driver_idx = i;
    drivers[i].initialize_device(usb_input);
    return;
  }
  // Use the Default driver
  usb_input->driver_idx = 0;
  drivers[0].initialize_device(usb_input);
}

void detach_driver(usb_input_t *usb_input) {
  drivers[usb_input->driver_idx].deinitialize_device(usb_input);
  usb_input->driver_idx = 0;
}

//========================================---
// Default Driver
//========================================---

void INIT_DRIVER(default_driver)(usb_input_t *usb_input) {

  usb_input->userData = NULL;
  tuh_hid_receive_report(usb_input->addr, usb_input->instance);

  usb_input->configs.num_of_configs = 0;
  usb_input->configs.configs = NULL;
  usb_input->connected = true;
}

void GET_DATA_DRIVER(default_driver)(usb_input_t *usb_input) {}

//========================================---
// Faceoff Driver
//========================================---

extern config_holder_t load_config(int fd);

bool IS_DRIVER(faceoff)(uint16_t vid, uint16_t pid) {
  return vid == 0x0e6f && pid == 0x0180;
}

static bool faceoff_ids[2] = {false, false};

extern const int faceoff_1_config_length;
extern const int faceoff_2_config_length;
extern const uint8_t faceoff_1_config[];
extern const uint8_t faceoff_2_config[];

void INIT_DRIVER(faceoff)(usb_input_t *usb_input) {

  INIT_DRIVER(default_driver)(usb_input);
  usb_input->userData = (void *)-1;
  for (long i = 0; i < 2; i++) {
    if (!faceoff_ids[i]) {
      faceoff_ids[i] = true;
      usb_input->userData = (void *)i;
      break;
    }
  }

  switch ((long)usb_input->userData) {
  case 0: {
    usb_input->configs.num_of_configs = 2;
    usb_input->configs.configs = (config_header_t **)malloc(2);
    long offset = 0;
    for (int i = 0; i < 2; i++) {
      uint8_t length = *faceoff_1_config;
      usb_input->configs.configs[i] = malloc(length);
      memcpy(usb_input->configs.configs[i], faceoff_1_config + offset, length);
      offset += length;
    }
  } break;
  case 1: {
    usb_input->configs.num_of_configs = 2;
    usb_input->configs.configs = (config_header_t **)malloc(2);
    long offset = 0;
    for (int i = 0; i < 2; i++) {
      uint8_t length = *faceoff_2_config;
      usb_input->configs.configs[i] = malloc(length);
      memcpy(usb_input->configs.configs[i], faceoff_2_config + offset, length);
      offset += length;
    }
  } break;
  }
}

void DEINIT_DRIVER(faceoff)(usb_input_t *usb_input) {
  switch ((long)usb_input->userData) {
  case 0:
  case 1:
    faceoff_ids[(long)usb_input->userData] = false;
  }
}

void GET_DATA_DRIVER(faceoff)(usb_input_t *usb_input) {
  GET_DATA_DRIVER(default_driver)(usb_input);
}

//========================================---
// Keyboard Driver
//========================================---

bool IS_DRIVER(keyboard)(uint16_t vid, uint16_t pid) {
  return vid == 0x045e && pid == 0x0745; // my wireless keyboard
}

void INIT_DRIVER(keyboard)(usb_input_t *usb_input) {
  INIT_DRIVER(default_driver)(usb_input);
  usb_input->configs = load_config(1);
}

void DEINIT_DRIVER(keyboard)(usb_input_t *usb_input) { (void)usb_input; }

void GET_DATA_DRIVER(keyboard)(usb_input_t *usb_input) {
  GET_DATA_DRIVER(default_driver)(usb_input);
}
