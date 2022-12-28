#include "drivers.h"
#include "configs.h"
#include "pio_usb.h"
#include "tusb.h"
#include <string.h>


const uint8_t __in_flash("drivers") num_of_drivers = 2;
const usb_driver_t __in_flash("drivers") drivers[] = {
  FACEOFF_PRO_CONTROLLER_DRIVER, 
  XBOX_WIRELESS_ADAPTER_DRIVER,
};

#define FACEOFF_DATA_LEN 8



extern uint32_t blink_interval_ms;

void attach_driver(hid_dev_t* dev, int dev_id) {
  for(int i = 0; i < num_of_drivers; i++) {
    if(drivers[i].is_driver_for_device(dev->_device->vid, dev->_device->pid)) {
      // set blink to mounted so that we get a visual on connected state
      blink_interval_ms = 1000;
      dev->driver_idx = i;
      drivers[i].initialize_device(dev, dev_id);
      break;
    }
  }
}


void detach_driver(hid_dev_t* dev) {
  if(dev->data != NULL) {
    free(dev->data); dev->data = NULL;
  }
  if(dev->config.num_of_elems > 0) {
    free(dev->config.elems);
    dev->config.elems = NULL;
    dev->config.num_of_elems = 0;
  }
  dev->data_len = 0;
  dev->driver_idx = 0;

  // Set blink interval to Unmounted so we get a visual on disconnect
  blink_interval_ms = 250;
}



//============================================================================---
//   FACEOFF_PRO_CONTROLLER  
//============================================================================---

bool faceoff_pro_controller_is_driver_for_device(uint16_t vid, uint16_t pid) {
  return vid == 0x0e6f && pid == 0x0180;
}

void copy_config(config_t* dest, config_t* src) {
  
  dest->num_of_elems = src->num_of_elems;
  dest->elems = malloc(sizeof(config_elem)*src->num_of_elems);
  for(int i = 0; i < src->num_of_elems; i++) {
    memcpy(&dest->elems[i], &src->elems[i], sizeof(src->elems[i]));
  }
}

void faceoff_pro_controller_initialize_device(hid_dev_t* device, int dev_id) {
  // initialize how much data the faceoff pro controller will use
  device->data_len = FACEOFF_DATA_LEN;
  device->data = (uint8_t*)malloc(FACEOFF_DATA_LEN);
  // Setup configs for controllers depending on connection id
  if(dev_id%2 == 0) {    
    //memcpy(&device->config, &left_risk_of_rain, sizeof(left_risk_of_rain));
    copy_config(&device->config, &pass_through_1);
    //memcpy(&device->config, &pass_through_1, sizeof(pass_through_1));
    //blink_interval_ms = 500;
  } else {
    //blink_interval_ms = 2500;
    copy_config(&device->config, &pass_through_2);
    //memcpy(&device->config, &pass_through_2, sizeof(pass_through_2));
    //memcpy(&device->config, &right_risk_of_rain, sizeof(right_risk_of_rain));
  }
}


void faceoff_pro_controller_get_data_for_device(hid_dev_t* device) {
  
  for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
    endpoint_t *ep = pio_usb_get_endpoint(device->_device, ep_idx);

    if (ep == NULL) {
      break;
    } 
    
      

    struct faceoff_pro_controller_data data;
    int len = pio_usb_get_in_data(ep, (uint8_t*)&data, sizeof(data));
    
    // Check that we grabbed the correct amount of data
    if(len == FACEOFF_DATA_LEN) {
      memcpy(device->data, (void*)&data, sizeof(data));
    }
  }
}


//============================================================================---
//   XBOX_WIRELESS_ADAPTER 
//============================================================================---
bool xbox_wireless_adapter_is_driver_for_device(uint16_t vid, uint16_t pid) {
  return vid == 0x045e && pid == 0x0719;
  //return true;
}

void xbox_wireless_adapter_initialize_device(hid_dev_t* device, int dev_id) {
  device->data_len = sizeof(xbox_controller)*4;
  device->data = malloc(sizeof(xbox_controller)*4); // xbox_controller[4]
  blink_interval_ms = 3000;
  copy_config(&device->config, &xbox_test_config);
  //memcpy(&device->config, &xbox_test_config, sizeof(xbox_test_config));
}

void xbox_wireless_adapter_get_data_for_device(hid_dev_t* device) {
  int cur_controller = 0;
  for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
    endpoint_t *ep = pio_usb_get_endpoint(device->_device, ep_idx);

    if (ep == NULL) {
      break;
    } 
    if(ep->is_tx && ep_idx % 4 < 2) {
      static uint8_t set_light_data[12] = {
        0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };
      set_light_data[3] = 0x46 + cur_controller;
      pio_usb_set_out_data(ep, set_light_data, sizeof(set_light_data));
      cur_controller += 1;
      continue;
    }
    static uint8_t temp[64] = {0x00};
    if(pio_usb_get_in_data(ep, temp, sizeof(temp)) <= 0) { continue; }

    if( ep_idx % 4 >= 2 ) { continue; }

    if(temp[1] != 0x01) { continue; }
    if(temp[3] != 0xf0) { continue; }
    if(temp[5] != 0x13) { continue; }
    xbox_controller* controller = &(((xbox_controller*)device->data)[cur_controller]);
    memset(controller, 0x00, sizeof(xbox_controller));
    controller->buttons = *((uint16_t*)&temp[6]);
    controller->lt = temp[8];
    controller->rt = temp[9];
    controller->axis_x = (*((int16_t*)&temp[10]))/0x100 + 0x80;
    controller->axis_y = (*((int16_t*)&temp[12]))/0x100 + 0x80;
    controller->axis_z = (*((int16_t*)&temp[14]))/0x100 + 0x80;
    controller->axis_rz = (*((int16_t*)&temp[16]))/0x100 + 0x80;
  }
}
