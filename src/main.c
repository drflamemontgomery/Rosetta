#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"

#include "pio_usb.h"
#include "tusb.h"

static usb_device_t *usb_device = NULL;

enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( (time_us_64()/1000) - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  gpio_put(25, led_state);
  //board_led_write(1);
  led_state = 1 - led_state; // toggle
}

void core1_main() {
  sleep_ms(10);

  // To run USB SOF interrupt in core1, create alarm pool in core1.
  static pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
  config.alarm_pool = (void*)alarm_pool_create(2, 1);
  usb_device = pio_usb_host_init(&config);

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
  
  multicore_reset_core1();
  multicore_launch_core1(core1_main);
  
  tud_init(0);



  while(1) {
    tud_task();
    led_blinking_task();
    if (usb_device != NULL) {
      for (int dev_idx = 0; dev_idx < PIO_USB_DEVICE_CNT; dev_idx++) {
        usb_device_t *device = &usb_device[dev_idx];
        if (!device->connected) {
          continue;
        }

        for (int ep_idx = 0; ep_idx < PIO_USB_DEV_EP_CNT; ep_idx++) {
          endpoint_t *ep = pio_usb_get_endpoint(device, ep_idx);

          if (ep == NULL) {
            break;
          }

          uint8_t temp[64];
          int len = pio_usb_get_in_data(ep, temp, sizeof(temp));

          if (len > 0) {
            if(tud_hid_n_ready(dev_idx)) {
              tud_hid_n_report(dev_idx, 0, temp, len);
            }
          }
        }
      } 
    }
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


void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_us = 1000;
  static uint32_t start_us = 0;

  if ( time_us_64() - start_us < interval_us) return; // not enough time
  start_us += interval_us;

  // Implement Stuff

}
