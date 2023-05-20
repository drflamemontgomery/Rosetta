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

output_controller_t hid_output[MAX_OUTPUTS] = {{
  0x0000,
  
  0x0f,

  0x00,
  0x00,
  0x00,
  0x00,

  0x00
}};

void hid_task(void) {
  // Poll every 10ms

  const uint32_t interval_us = 1000;
  static uint32_t start_us = 0;

  if(time_us_64() - start_us < interval_us) return;
  start_us += interval_us;

  for(int i = 0; i < MAX_OUTPUTS; i++) {
    if(!tud_hid_n_ready(i)) continue;
    tud_hid_n_report(i, 0, (uint8_t*)&hid_output[i], sizeof(*hid_output));
  }
}
