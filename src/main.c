#include "pico/multicore.h"
#include "hardware/watchdog.h"

#include "tusb.h"

extern void setup(void); // setup.c
extern void core1_main(void); // usb_host.c
extern void usb_host_task(void); // usb_host.c
extern void hid_task(void); // output.c
extern void led_blinking_task(void); // led.c

int main(void) {
  setup();


  multicore_reset_core1();
  multicore_launch_core1(core1_main);

  tud_init(0);

  while(true) {
    tud_task();
    led_blinking_task();
    usb_host_task();
    // HID OUTPUT TASK
    hid_task();
    watchdog_update(); 
  }
  return 0;
}
