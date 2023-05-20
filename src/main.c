#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/irq.h"
#include "hardware/watchdog.h"
#include "pico/binary_info.h"

#include "pio_usb.h"
#include "tusb.h"

extern void init(void);
extern void core1_main(void); // located in usb_host.c
extern void usb_host_task(void); 

int main(void) {

  init();

  multicore_reset_core1();
  multicore_launch_core1(core1_main);

  tud_init(0);

  while(true) {
    tud_task();
    usb_host_task();
    // HID OUTPUT TASK
    watchdog_update(); 
  }

  return 0;
}
