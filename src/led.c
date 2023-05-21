#include "pico/stdlib.h"
#include "settings.h"

uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void) {

  static uint32_t start_ms = 0;
  static bool led_state = false;

  if((time_us_64()/1000) - start_ms < blink_interval_ms) return;
  start_ms += blink_interval_ms;

  gpio_put(GPIO_LED, led_state);
  led_state = !led_state;
} 
