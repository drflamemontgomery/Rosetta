#pragma once
#ifndef ROSETTA_DRIVERS
#define ROSETTA_DRIVERS

#include "hid_dev.h"
#include "pio_usb.h"
#include <stdlib.h>
#include <pico/platform.h>



//============================================================================---
//   FACEOFF_PRO_CONTROLLER  
//============================================================================---
bool faceoff_pro_controller_is_driver_for_device(uint16_t vid, uint16_t pid);
void faceoff_pro_controller_initialize_device(hid_dev_t* device, int dev_id);
void faceoff_pro_controller_get_data_for_device(hid_dev_t* device);
static const usb_driver_t FACEOFF_PRO_CONTROLLER_DRIVER = {
  .is_driver_for_device = faceoff_pro_controller_is_driver_for_device,
  .initialize_device = faceoff_pro_controller_initialize_device,
  .get_data_for_device = faceoff_pro_controller_get_data_for_device,
};

//============================================================================---
//   XBOX_WIRELESS_ADAPTER 
//============================================================================---
bool xbox_wireless_adapter_is_driver_for_device(uint16_t vid, uint16_t pid);
void xbox_wireless_adapter_initialize_device(hid_dev_t* device, int dev_id);
void xbox_wireless_adapter_get_data_for_device(hid_dev_t* device);
static const usb_driver_t XBOX_WIRELESS_ADAPTER_DRIVER = {
  .is_driver_for_device = xbox_wireless_adapter_is_driver_for_device,
  .initialize_device = xbox_wireless_adapter_initialize_device,
  .get_data_for_device = xbox_wireless_adapter_get_data_for_device,
};

//============================================================================---
//   DRIVERS  
//============================================================================---

extern const uint8_t __in_flash("drivers") num_of_drivers;
extern const usb_driver_t __in_flash("drivers") drivers[];

void attach_driver(hid_dev_t* dev, int dev_id);
void detach_driver(hid_dev_t* dev);

void apply_config_for_device(hid_dev_t* dev);
#endif
