#include "hid_dev.h"

extern uint32_t blink_interval_ms;

#define HAT_RIGHT (1<<0)
#define HAT_LEFT  (1<<1)
#define HAT_DOWN  (1<<2)
#define HAT_UP    (1<<3)

uint8_t hat_to_dir(uint8_t hat) {
  switch(hat) {
    case 0x00:
      return HAT_UP;
    case 0x01:
      return HAT_UP | HAT_RIGHT;
    case 0x02:
      return HAT_RIGHT;
    case 0x03:
      return HAT_RIGHT | HAT_DOWN;
    case 0x04:
      return HAT_DOWN;
    case 0x05:
      return HAT_DOWN | HAT_LEFT;
    case 0x06:
      return HAT_LEFT;
    case 0x07:
      return HAT_LEFT | HAT_UP;
  }
  return 0x00;
}

void run_config_holder(input_dev_t* dev) {
  for(int idx = 0; idx < dev->num_of_devices; idx++) {
    if(dev->devices_idx[idx] < 0) { continue; }
    device_t* device = &attached_devices[dev->devices_idx[idx]];
    if(!device->connected) { continue; }

    uint8_t* data = device->data;
    config_holder* configs = &device->new_configs;
    
    for(int i = 0; i < configs->num_of_configs; i++) {
      bool active = false;
      
      config_header* config = configs->configs[i];

      uint8_t *config_data = (uint8_t*)(config) + 6;

      switch(config->active_type) {
        case CH_BUTTON_PASS_THROUGH:
        case CH_HAT_PASS_THROUGH:
        case CH_AXIS_PASS_THROUGH:
        case CH_USER_DEFINED:
          active = true;
          break;
        case CH_BUTTON:
          //active =
          //TODO implement button detection
          config_data += config->byte_size;
          break;
        case CH_HAT:
          uint8_t hat = hat_to_dir(data[config->start_byte]);
          uint8_t hat_data = *config_data++;
          active = (hat & hat_data) != hat_data;
          break;
        case CH_AXIS:
          //TODO implement axis detection
          config_data += config->byte_size;
          break;
      }

      if(!active) continue;

      switch(config->mapping_type) {
        case CH_BUTTON_PASS_THROUGH:
          hid_device_out[config->controller_id].buttons |= *((uint16_t*)(data + config->start_byte));
          break;
        case CH_AXIS_PASS_THROUGH:
          *((uint8_t*)&hid_device_out[config->controller_id].axis_x + (*config_data))
            = *((uint8_t*)(data + config->start_byte));
          break;
        case CH_HAT_PASS_THROUGH:
          hid_device_out[config->controller_id].hat = *((uint8_t*)(data + config->start_byte));
          break;
        case CH_BUTTON:
          hid_device_out[config->controller_id].buttons |= *((uint16_t*)config_data);
          break;
        case CH_HAT:
          // TODO implement better hat
          break;
        case CH_AXIS:
          *((uint8_t*)&hid_device_out[config->controller_id].axis_x + (*config_data)) = *(config_data + 1);
          
          break;
        case CH_USER_DEFINED:
          // TODO implement user defined
          break;
      }
    }
  }
}
