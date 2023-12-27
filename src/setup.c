#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "settings.h"

//====================================================================---
//  EXTERNAL FUNCTIONS
//====================================================================---

extern void screen_init();

//====================================================================---
//  MAIN SETUP CODE
//====================================================================---

void on_watchdog_reboot(void);

void setup(void) {
  set_sys_clock_khz(120000, true); // Set the clock for USB

  // Turn the board LED on
  gpio_init(GPIO_LED);
  gpio_set_dir(GPIO_LED, GPIO_OUT);
  gpio_put(GPIO_LED, 1);

  if(watchdog_caused_reboot()) on_watchdog_reboot();

  // setup watchdog for auto rebooting
  watchdog_enable(WATCHDOG_TIMEOUT, WATCHDOG_DEBUG);

  screen_init();
}

void on_watchdog_reboot(void) {
  // TODO implement reboot
}
