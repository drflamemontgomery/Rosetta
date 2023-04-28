#include "drivers.h"
#include "configs.h"
#include "pio_usb.h"
#include "tusb.h"
#include <string.h>


const uint8_t __in_flash("drivers") num_of_drivers = 3;
const usb_driver_t __in_flash("drivers") drivers[] = {
  FACEOFF_PRO_CONTROLLER_DRIVER, 
  XBOX_WIRELESS_ADAPTER_DRIVER,
  PS1_RACING_WHEEL_ADAPTER_DRIVER,
};

#define FACEOFF_DATA_LEN 8



extern uint32_t blink_interval_ms;

void attach_driver(input_dev_t* dev) {
  for(int i = 0; i < num_of_drivers; i++) {
    if(drivers[i].is_driver_for_device(dev->_device->vid, dev->_device->pid)) {
      // set blink to mounted so that we get a visual on connected state
      blink_interval_ms = 1000;
      dev->driver_idx = i;
      drivers[i].initialize_device(dev);
      break;
    }
  }
}

void deinitialize_device(device_t* device) {
  device->connected = false;
  device->data_len = 0;
  if(device->data != NULL) {
    free(device->data);
    device->data = NULL;
  }

  /*
   * DO NOT FREE CONFIG MEMORY
   * IT IS POINTING TO A PRECONFIGURED CONFIG
   */
  device->config = NULL;
}

void detach_driver(input_dev_t* dev) {
  for(int i = 0; i < dev->num_of_devices; i++) {
    deinitialize_device(&attached_devices[dev->devices_idx[i]]);
  }
  if(dev->devices_idx != NULL) {
    free(dev->devices_idx);
    dev->devices_idx = NULL;
  }
  dev->num_of_devices = 0;
  dev->driver_idx = 0;
  // Set blink interval to Unmounted so we get a visual on disconnect
  //blink_interval_ms = 250;
}

int find_next_free_device_idx() {
  for(int i = 0; i < MAX_ATTACHED_DEVICES; i++) {
    if(!attached_devices[i].connected) { return i; }
  }
  return -1;
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



void faceoff_pro_controller_initialize_device(input_dev_t* device) {
  int next_id = find_next_free_device_idx();
  if(next_id < 0) { return; }
  
  device->num_of_devices = 1;
  device->devices_idx = malloc(sizeof(int));
  *device->devices_idx = next_id;
  
  device_t* attached_dev = &attached_devices[next_id];
  
  attached_dev->data_len = sizeof(pro_controller_data);
  attached_dev->data = malloc(sizeof(pro_controller_data));
  if(next_id % 4 == 0) {
    attached_dev->config = &pass_through_1;
  }
  else if(next_id % 4 == 1) {
    attached_dev->config = &pass_through_2;
  }

  attached_dev->connected = true;
}


void faceoff_pro_controller_get_data_for_device(input_dev_t* device) {
  if(!device->_device->connected) { return; }
  for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
    endpoint_t *ep = pio_usb_get_endpoint(device->_device, ep_idx);

    if (ep == NULL) {
      break;
    } 

    struct faceoff_pro_controller_data data;
    int len = pio_usb_get_in_data(ep, (uint8_t*)&data, 64);
    if(len > 0) { 
      device_t* dev = &attached_devices[device->devices_idx[0]];
      struct faceoff_pro_controller_data* dev_data = (struct faceoff_pro_controller_data*)dev->data;
      dev_data->buttons = data.buttons;
      dev_data->hat = data.hat;
      dev_data->axis_x = data.axis_x;
      dev_data->axis_y = data.axis_y;
      dev_data->axis_z = data.axis_z;
      dev_data->axis_rz = data.axis_rz;
      //memcpy(attached_devices[device->devices_idx[0]].data, (void*)&data, sizeof(data));
    }
  }
}

//============================================================================---
//   XBOX_WIRELESS_ADAPTER 
//============================================================================---
bool xbox_wireless_adapter_is_driver_for_device(uint16_t vid, uint16_t pid) {
  return vid == 0x045e && pid == 0x0719;
}

void xbox_wireless_adapter_initialize_device(input_dev_t* device) {
  device->num_of_devices = 4;
  device->devices_idx = malloc(sizeof(int)*4);
  for(int i = 0; i < 4; i++) {
    int next_id = find_next_free_device_idx();
    if(next_id < 0) { break; }

    device->devices_idx[i] = next_id;

    device_t* attached_dev = &attached_devices[next_id];

    attached_dev->data_len = sizeof(xbox_controller);
    attached_dev->data = malloc(sizeof(xbox_controller));
    xbox_controller* controller = (xbox_controller*)attached_dev->data;
    controller->rumble_left = 0x00;
    controller->rumble_right = 0x00;

    if(i == 0) {
      attached_dev->config = &xbox_pass_1;
    }
    else if( i == 1) {
      attached_dev->config = &xbox_pass_2; 
    }
    else {
      attached_dev->config = NULL;
    }

    attached_dev->connected = true;
  }
}

void xbox_wireless_adapter_get_data_for_device(input_dev_t* device) {
  int cur_controller = 0;
  for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
    endpoint_t *ep = pio_usb_get_endpoint(device->_device, ep_idx);

    if (ep == NULL) {
      break;
    } 
    

    if(ep->is_tx && ep_idx % 4 < 2) {
      xbox_controller* controller = (xbox_controller*)attached_devices[device->devices_idx[cur_controller]].data;

      static uint8_t set_rumble_data[] = {
        0x00, 0x01, 0x00, 0x0c, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
      };
      set_rumble_data[5] = controller->rumble_left;
      set_rumble_data[6] = controller->rumble_right;
      pio_usb_set_out_data(ep, set_rumble_data, sizeof(set_rumble_data));
      
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
    if(device->devices_idx[cur_controller] < 0) { continue; }

    if( ep_idx % 4 >= 2 ) { continue; }

    if(temp[1] != 0x01) { continue; }
    if(temp[3] != 0xf0) { continue; }
    if(temp[5] != 0x13) { continue; }

    xbox_controller* controller = (xbox_controller*)attached_devices[device->devices_idx[cur_controller]].data;
    
    controller->buttons = *((uint16_t*)&temp[6]);
    controller->lt = temp[8];
    controller->rt = temp[9];
    controller->axis_x = (*((int16_t*)&temp[10]))/0x100 + 0x80;
    controller->axis_y = (*((int16_t*)&temp[12]))/0x100 + 0x80;
    controller->axis_z = (*((int16_t*)&temp[14]))/0x100 + 0x80;
    controller->axis_rz = (*((int16_t*)&temp[16]))/0x100 + 0x80;
    //controller->rumble_left = controller->lt;
    //controller->rumble_right = controller->rt;
  }
}



//============================================================================---
//   PS1_RACING_WHEEL_ADAPTER 
//============================================================================---
bool ps1_racing_wheel_adapter_is_driver_for_device(uint16_t vid, uint16_t pid) {
  return vid == 0x054c && pid == 0x0268;
}

void ps1_racing_wheel_adapter_initialize_device(input_dev_t* device) {
  device->num_of_devices = 1;
  device->devices_idx = malloc(sizeof(int));
  int next_id = find_next_free_device_idx();
  if(next_id < 0) { return; }

  *device->devices_idx = next_id;

  device_t* attached_dev = &attached_devices[next_id];

  attached_dev->data_len = sizeof(ps1_racing_wheel);
  attached_dev->data = malloc(sizeof(ps1_racing_wheel));
  ps1_racing_wheel* controller = (ps1_racing_wheel*)attached_dev->data;
  controller->axis_x = 0x80;
  controller->axis_y = 0x80;
  controller->paddle = 0x80;
  controller->buttons = 0x0000;

  attached_dev->config = &ps1_racing_wheel_pass;

  attached_dev->connected = true;
}

void ps1_racing_wheel_adapter_get_data_for_device(input_dev_t* device) {
  for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
    endpoint_t *ep = pio_usb_get_endpoint(device->_device, ep_idx);

    if (ep == NULL) {
      break;
    } 
    
    if (ep->is_tx) { continue; }

    struct {
      uint16_t id;
      uint16_t buttons;
      uint16_t null_bytes;
      uint8_t axis_x;
      uint8_t paddle;
      uint8_t trail_bytes[0x29]; 
    } temp;

    if(pio_usb_get_in_data(ep, (uint8_t*)&temp, sizeof(temp)) <= 0) { continue; }
    if(*device->devices_idx < 0) { continue; }
    
    ps1_racing_wheel* controller = (ps1_racing_wheel*)attached_devices[*device->devices_idx].data;
    controller->buttons = temp.buttons;
    controller->paddle = temp.paddle;  
    controller->axis_x = temp.axis_x;
    controller->axis_y = 0x80 - (temp.trail_bytes[6] ? 0x7f : 0x00) + (temp.trail_bytes[8] ? 0x7f : 0x00);

    //blink_interval_ms = 5000;
  }
}
