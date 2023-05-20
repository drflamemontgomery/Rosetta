#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/irq.h"
#include "pico/binary_info.h"

#include "pio_usb.h"
#include "tusb.h"

#include "settings.h"
#include "structs.h"

extern void attach_driver(usb_input_t* usb_input);
extern void detach_driver(usb_input_t* usb_input);

usb_device_t* usb_device = NULL;

int num_of_input_devices = 0;
usb_input_t usb_devices[MAX_USB_INPUTS] = {NULL};


void setup_usb_host(void);

void core1_main(void) {
  setup_usb_host();
  while(true) pio_usb_host_task();
}


void setup_usb_host(void) {

  static pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
  config.pin_dp = USB_HOST_PIN_1;
  config.alarm_pool = (void*)alarm_pool_create(2, 1);
  usb_device = pio_usb_host_init(&config);

#if USB_HOST_ADD_PORT
  pio_usb_host_add_port(USB_HOST_PIN_2);
#endif
}

void flush_hubs(void);
extern void update_usb_drivers(usb_device_t *usb_device);

void usb_host_task(void) {
  flush_hubs();
  update_usb_drivers(usb_device);
}


// Get Data From USB Hubs
void flush_hubs(void) {
  if(usb_device != NULL) {
    for(int dev_idx = 0; dev_idx < PIO_USB_DEVICE_CNT; dev_idx++) {
      usb_device_t *device = &usb_device[dev_idx];

      if(!device->connected) continue;
      if(device->device_class != CLASS_HUB) continue;

      for(int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
        endpoint_t *ep = pio_usb_get_endpoint(device, ep_idx);

        if(ep == NULL) break;

        static uint8_t temp[64];
        pio_usb_get_in_data(ep, temp, sizeof(temp));
      }
    }
  }
}

void pio_hid_connect_host_cb(usb_device_t *device) {
  if(device == NULL) return;
  if(device->device_class == CLASS_HUB) return;

  for(int i = 0; i < MAX_USB_INPUTS; i++) {
    if(usb_devices[i]._device != NULL) continue;
    usb_devices[i]._device = device;

    attach_driver(&usb_devices[i]);
    num_of_input_devices += 1;
    return;
  }
}

void pio_disconnect_host_cb(usb_device_t *device) {
  if(device == NULL) return;
  if(device->device_class == CLASS_HUB) return;

  for(int i = 0; i < MAX_USB_INPUTS; i++) {
    if(usb_devices[i]._device != device) continue;
    usb_devices[i]._device = NULL;

    detach_driver(&usb_devices[i]);
    num_of_input_devices -= 1;
    return;
  }
}
