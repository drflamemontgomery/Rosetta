#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"

#include "pio_usb.h"
#include "tusb.h"

#include "drivers.h"
#include "configs.h"

#define MAX_HID_DEVICES 4

uint8_t num_of_input_devices = 0;
// Abstraction for holding current plugged in devices with their drivers

input_dev_t input_devices[MAX_INPUT_DEVICES]  = {{ NULL, 0, NULL, 0 }};
device_t attached_devices[MAX_ATTACHED_DEVICES] = {{ false, 0x00, NULL, NULL }};

uint8_t true_out[4] = {0, 1, 2, 3};

static usb_device_t *usb_device = NULL;

// Data used by device drivers for interacting as a usb device
pro_controller_data hid_device_out[MAX_HID_OUT] = {
  {0x0000, 0x0f, 0x80, 0x80, 0x80, 0x80} 
};


enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void usb_host_task();
void hid_task(void);

void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( (time_us_64()/1000) - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  gpio_put(25, led_state);
  led_state = 1 - led_state; // toggle
}

void core1_main() {
  sleep_ms(10);

  // To run USB SOF interrupt in core1, create alarm pool in core1.
  static pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
  config.pin_dp = 2;
  config.alarm_pool = (void*)alarm_pool_create(2, 1);
  usb_device = pio_usb_host_init(&config);
  pio_usb_host_add_port(6);

  while (true) {
    pio_usb_host_task();
  }
}

int main(void) {

  set_sys_clock_khz(120000, true);
  sleep_ms(10);
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  gpio_put(25, 1);
 
  // initialize controller configs so that they are ready for use by drivers 
  init_configs(); 
  
  multicore_reset_core1();
  multicore_launch_core1(core1_main);
  

  tud_init(0);

  srand(time_us_64());

  while(1) {
    tud_task();
    led_blinking_task();

    // Get the reports from HUB devices so that we can access the HID device reports
    if (usb_device != NULL) {
      for (int dev_idx = 0; dev_idx < PIO_USB_DEVICE_CNT; dev_idx++) {
        usb_device_t *device = &usb_device[dev_idx];
        if (!device->connected) {
          continue;
        }
        if(device->device_class != CLASS_HUB) {
          continue;
        }


        for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
          endpoint_t *ep = pio_usb_get_endpoint(device, ep_idx);

          if (ep == NULL) {
            break;
          }
          
          // Update HUB data which polls the Hubs USB Devices
          static uint8_t temp[64];
          pio_usb_get_in_data(ep, temp, sizeof(temp));
        }
      } 
    }

    // Update USB Devices before outputting the report
    usb_host_task();
    hid_task();
  }

  return 0;
}


// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen; 
  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) bufsize; 

}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

void switch_players() {
  for(int i = 0; i < 4; i++) {
    int p = rand()%4;

    bool changed = true;
    while(changed) {
      changed = false;
      for(int j = 0; j < i; j++) {
        if(p == true_out[j]) {
          changed = true;
          p = (p + 1)%4;
        }
      }
    }

    true_out[i] = p;
  }
}

void check_switch() {
  static bool is_pressed[4] = { false };

  for(int i = 0; i < 4; i++) {
#define should_switch (0b11<<10)
    if((hid_device_out[i].buttons & should_switch) == should_switch && !is_pressed[i]) {
      is_pressed[i] = true; 
      switch_players();
    }
    else if(is_pressed[i] && (hid_device_out[i].buttons & should_switch) != should_switch) {
      is_pressed[i] = false;
    }
  }
}

void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_us = 1000;
  static uint32_t start_us = 0;

  if ( time_us_64() - start_us < interval_us) return; // not enough time
  start_us += interval_us;
 
  check_switch();
  // Output reports for all devices
  for(int i = 0; i < 4; i++) {
    if(tud_hid_n_ready(true_out[i])) {
      tud_hid_n_report(true_out[i], 0, (uint8_t*)&hid_device_out[i], sizeof(pro_controller_data));
    }
  }
}

// Callback used for adding USB devices to our abstract layer
void pio_hid_connect_host_cb(usb_device_t *device) {
  if(device == NULL) {return;}
  if(device->device_class == CLASS_HUB) {return;}
  
  for(int i = 0; i < MAX_INPUT_DEVICES; i++) {
    if(input_devices[i]._device != NULL) { continue; }
    input_devices[i]._device = device;
    attach_driver(&input_devices[i]);
    num_of_input_devices += 1;
    return;
  }
}

void pio_disconnect_host_cb(usb_device_t *device) {
  if(device == NULL) {return;}
  if(device->device_class == CLASS_HUB) {return;}
  
  for(int i = 0; i < MAX_INPUT_DEVICES; i++) {
    if(input_devices[i]._device != device) { continue; }
    input_devices[i]._device = NULL;
    detach_driver(&input_devices[i]);
    num_of_input_devices -= 1;
    return;
  }
}


void usb_host_task() {
  if(usb_device == NULL) { return; }


  // Reset Report Data so that we can change the report 
  for(int i = 0; i < MAX_HID_OUT; i++) {
    static const pro_controller_data default_data = {0x0000, 0x0f, 0x80, 0x80, 0x80, 0x80};
    memcpy(&hid_device_out[i], &default_data, sizeof(default_data));
  }

  uint8_t found_devices = 0;
  for(int i = 0; i < MAX_INPUT_DEVICES; i++) {
    if(found_devices >= num_of_input_devices) { break; }
    if(input_devices[i]._device == NULL) { continue; }
    
    found_devices++;
    input_dev_t* dev = &input_devices[i];
    
    if(!dev->_device->connected) {continue;}
    if(dev->_device->device_class == CLASS_HUB) {continue;}
    // Run driver then config
    drivers[dev->driver_idx].get_data_for_device(dev);
    run_config(dev); 
  }
}
