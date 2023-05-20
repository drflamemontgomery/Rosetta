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

void update_usb_drivers(usb_device_t *usb_device) {
  (void) usb_device;
  // TODO implement update
}
