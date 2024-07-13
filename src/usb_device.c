#include "pico/stdlib.h"
#include "tusb.h"

#include "settings.h"
#include "structs.h"

output_controller_t hid_output[MAX_OUTPUTS] = {{0x0000,

                                                0x0f,

                                                0x00, 0x00, 0x00, 0x00,

                                                0x00}};

void hid_task(void) {
  // Poll every 10ms

  const uint32_t interval_us = 1000;
  static uint32_t start_us = 0;

  if (time_us_64() - start_us < interval_us)
    return;
  start_us += interval_us;

  for (int i = 0; i < MAX_OUTPUTS; i++) {
    if (!tud_hid_n_ready(i))
      continue;
    tud_hid_n_report(i, 0, (uint8_t *)&hid_output[i], sizeof(*hid_output));
  }
}
// Invoked when device is mounted
void tud_mount_cb(void) {}

// Invoked when device is unmounted
void tud_umount_cb(void) {}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;
  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)bufsize;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) { (void)remote_wakeup_en; }

// Invoked when usb bus is resumed
void tud_resume_cb(void) {}
