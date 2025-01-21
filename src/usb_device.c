#include "pico/stdio.h"
#include "tusb.h"

#include "usb.h"

void UsbOutput_sendReport(const UsbOutput *output) {
  if (!tud_hid_n_ready(output->id))
    return;
  union {
    struct {
      uint16_t buttons;
      uint8_t hat;
      uint8_t axis_x;
      uint8_t axis_y;
      uint8_t axis_z;
      uint8_t axis_rz;
    };
    uint8_t bytes[8];
  } outputBytes = {
      .buttons = output->report.buttons,
      .hat = 0xf,
      .axis_x = output->report.axis_x,
      .axis_y = output->report.axis_y,
      .axis_z = output->report.axis_z,
      .axis_rz = output->report.axis_rz,
  };

  // Create the correct hat byte from the direction data
  switch (output->report.hat) {
  case 0b0100:
  case 0b0111:
    // North
    outputBytes.hat = 0x0;
    break;
  case 0b0110:
    // North East
    outputBytes.hat = 0x1;
    break;
  case 0b0010:
  case 0b1110:
    // East
    outputBytes.hat = 0x2;
    break;
  case 0b1010:
    // South East
    outputBytes.hat = 0x3;
    break;
  case 0b1000:
  case 0b1011:
    // South
    outputBytes.hat = 0x4;
    break;
  case 0b1001:
    // South West
    outputBytes.hat = 0x5;
    break;
  case 0b0001:
  case 0b1101:
    // West
    outputBytes.hat = 0x6;
    break;
  case 0b0101:
    // North West
    outputBytes.hat = 0x7;
    break;
  }

  tud_hid_n_report(output->id, 0, outputBytes.bytes, sizeof(outputBytes.bytes));
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
