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

#include "settings.h"

//====================================================================---
//  EXTERNAL FUNCTIONS
//====================================================================---

//====================================================================---
//  MAIN SETUP CODE
//====================================================================---

void on_watchdog_reboot(void);

void init(void) {
  set_sys_clock_khz(12000, true); // Set the clock for USB
  

  // Turn the board LED on
  gpio_init(GPIO_LED);
  gpio_set_dir(GPIO_LED, GPIO_OUT);
  gpio_put(GPIO_LED, 1);

  if(watchdog_caused_reboot()) on_watchdog_reboot();

  // setup watchdog for auto rebooting
  watchdog_enable(WATCHDOG_TIMEOUT, WATCHDOG_DEBUG);

}
