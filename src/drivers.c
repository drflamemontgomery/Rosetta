#include "drivers.h"
#include "configs.h"
#include "pio_usb.h"
#include "tusb.h"
#include <string.h>


const uint8_t __in_flash("drivers") num_of_drivers = 1;
const usb_driver_t __in_flash("drivers") drivers[] = {
  FACEOFF_PRO_CONTROLLER_DRIVER, 
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

bool faceoff_pro_controller_is_driver_for_device(uint16_t vid, uint16_t pid) {
  return vid == 0x0e6f && pid == 0x0180;
}


void faceoff_pro_controller_initialize_device(hid_dev_t* device, int dev_id) {
  // initialize how much data the faceoff pro controller will use
  device->data_len = FACEOFF_DATA_LEN;
  device->data = (uint8_t*)malloc(FACEOFF_DATA_LEN);
  // Setup configs for controllers depending on connection id
  if(dev_id%2 == 0) {    
    memcpy(&device->config, &left_risk_of_rain, sizeof(left_risk_of_rain));
    blink_interval_ms = 500;
  } else {
    blink_interval_ms = 2500;
    memcpy(&device->config, &right_risk_of_rain, sizeof(right_risk_of_rain));
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
