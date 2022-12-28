#include "hid_dev.h"

extern uint32_t blink_interval_ms;

void run_button_config(uint8_t byte, config_elem* config) {

  if((byte & config->btn.mask_from) == config->btn.mask_from) {
    hid_device_out[config->output].buttons |= config->btn.mask_to;
  }

}

void run_axis_config(uint8_t byte, config_elem* config) {
  if(config->axis.reversed == 1) {
    byte = 0xFF - byte;
  }

  switch(config->axis.axis) {
    case 0:
      hid_device_out[config->output].axis_x = byte;
      break;
    case 1:
      hid_device_out[config->output].axis_y = byte;
      break;
    case 2:
      hid_device_out[config->output].axis_z = byte;
      break;
    case 3:
      hid_device_out[config->output].axis_rz = byte;
      break;
  }
}

void run_config(hid_dev_t* dev) {
  
  uint8_t* data = dev->data;
  config_t* config = &dev->config;

  for(int i = 0; i < config->num_of_elems; i++) {
    config_elem* elem = &config->elems[i];

    switch(elem->type) {
      case BUTTON:
        run_button_config(data[elem->byte], elem);
        break;
      case HAT:
        hid_device_out[elem->output].hat = data[elem->byte];
        break;
      case AXIS:
        run_axis_config(data[elem->byte], elem);
        break;
      case USER_DEFINED:
        elem->user.run_config(elem, data);
        break;
      default:
        break;
    }
  } 
}
