#include "settings.h"
#include "structs.h"

#define HAT_RIGHT (1 << 0)
#define HAT_LEFT (1 << 1)
#define HAT_DOWN (1 << 2)
#define HAT_UP (1 << 3)

uint8_t hat_to_dir(uint8_t hat) {
  switch (hat) {
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

extern usb_input_t usb_devices[MAX_USB_INPUTS];
extern output_controller_t hid_output[MAX_OUTPUTS];

void run_config_holder(usb_input_t *usb_input) {
  if (!usb_input->connected)
    return;
  uint8_t *data = usb_input->data;
  if (data == NULL)
    return;
  uint8_t data_len = usb_input->data_len;
  if (data_len <= 0)
    return;
  config_holder_t *configs = &usb_input->configs;

  for (int i = 0; i < configs->num_of_configs; i++) {
    bool active = false;

    config_header_t *config = configs->configs[i];

    uint8_t *config_data = (uint8_t *)(config) + 6;

    switch (config->active_type) {
    case CH_BUTTON_PASS_THROUGH:
    case CH_HAT_PASS_THROUGH:
    case CH_AXIS_PASS_THROUGH:
    case CH_USER_DEFINED:
      active = true;
      break;
    case CH_BUTTON:
      // active =
      // TODO implement button detection
      config_data += config->byte_size;
      break;
    case CH_HAT:
      if (config->start_byte >= data_len)
        break;
      uint8_t hat = hat_to_dir(data[config->start_byte]);
      uint8_t hat_data = *config_data++;
      active = (hat & hat_data) != hat_data;
      break;
    case CH_AXIS:
      // TODO implement axis detection
      config_data += config->byte_size;
      break;
    }

    if (!active)
      continue;

    switch (config->mapping_type) {
    case CH_BUTTON_PASS_THROUGH:
      if (config->start_byte + 1 >= data_len)
        break;
      hid_output[config->controller_id].buttons |=
          *((uint16_t *)(data + config->start_byte));
      break;
    case CH_AXIS_PASS_THROUGH:
      if (config->start_byte >= data_len)
        break;
      *((uint8_t *)&hid_output[config->controller_id].axis_x + (*config_data)) =
          *((uint8_t *)(data + config->start_byte));
      break;
    case CH_HAT_PASS_THROUGH:
      if (config->start_byte >= data_len)
        break;
      hid_output[config->controller_id].hat =
          *((uint8_t *)(data + config->start_byte));
      break;
    case CH_BUTTON:
      if (config->start_byte + 1 >= data_len)
        break;
      hid_output[config->controller_id].buttons |= *((uint16_t *)config_data);
      break;
    case CH_HAT:
      // TODO implement better hat
      break;
    case CH_AXIS:
      *((uint8_t *)&hid_output[config->controller_id].axis_x + (*config_data)) =
          *(config_data + 1);

      break;
    case CH_USER_DEFINED:
      // TODO implement user defined
      break;
    }
  }
}
