#include "settings.h"
#include "structs.h"
#include <stdbool.h>

#include "pio_usb.h"
#include "tusb.h"

#include "text.h"

extern void attach_driver(usb_input_t *usb_input);
extern void detach_driver(usb_input_t *usb_input);

void usb_host_task(void);
extern frame_buffer_t screen;

int num_of_input_devices = 0;
usb_input_t usb_devices[MAX_USB_INPUTS] = {NULL};

void setup_usb_host(void);

void core1_main(void) {
  setup_usb_host();
  while (true) {
    tuh_task();
    usb_host_task();
  }
}

void setup_usb_host(void) {

  static pio_usb_configuration_t config = PIO_USB_DEFAULT_CONFIG;
  config.pin_dp = USB_HOST_PIN_1;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &config);
#if USB_HOST_ADD_PORT
  static pio_usb_configuration_t config2 = PIO_USB_DEFAULT_CONFIG;
  config.pin_dp = USB_HOST_PIN_2;
  tuh_configure(2, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &config2);
  // pio_usb_host_add_port(USB_HOST_PIN_2, PIO_USB_PINOUT_DPDM);
#endif

  tuh_init(1);
#if USB_HOST_ADD_PORT
  tuh_init(2);
#endif
}

void usb_host_task(void) {
  for (int i = 0; i < MAX_USB_INPUTS; i++) {
    if (!usb_devices[i].connected)
      continue;
    if (!tuh_hid_mounted(usb_devices[i].addr, usb_devices[i].instance))
      continue;
    tuh_hid_receive_report(usb_devices[i].addr, usb_devices[i].instance);
  }
}

void tuh_mount_cb(uint8_t dev_addr) { (void)dev_addr; }
void tuh_umount_cb(uint8_t dev_addr) { (void)dev_addr; }
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance,
                      uint8_t const *desc_report, uint16_t desc_len) {
  char buf[64];
  sprintf(buf, "[HID %02X:%02X] Connected\n", dev_addr, instance);
  append_frame_buffer(&screen, (const char *)buf);

  (void)desc_report;
  (void)desc_len;

  for (int i = 0; i < MAX_USB_INPUTS; i++) {
    if (usb_devices[i].connected)
      continue;
    usb_devices[i].connected = true;
    usb_devices[i].addr = dev_addr;
    usb_devices[i].instance = instance;

    attach_driver(&usb_devices[i]);
    num_of_input_devices += 1;
    return;
  }
}
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  char buf[64];
  sprintf(buf, "[HID %02X:%02X] Disconnected\n", dev_addr, instance);
  append_frame_buffer(&screen, (const char *)buf);
  (void)dev_addr;
  (void)instance;
  for (int i = 0; i < MAX_USB_INPUTS; i++) {
    if (!usb_devices[i].connected)
      continue;
    if (usb_devices[i].addr != dev_addr)
      continue;
    if (usb_devices[i].instance != instance)
      continue;
    usb_devices[i].connected = false;
    detach_driver(&usb_devices[i]);
    num_of_input_devices -= 1;
    return;
  }
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance,
                                uint8_t const *report, uint16_t len) {
  char buf[64] = {0};
  sprintf(buf, "[%02X:%02X]:", dev_addr, instance);
  append_frame_buffer(&screen, (const char *)buf);
  for (int i = 0; i < len; i++) {
    sprintf(buf, " %02X", report[i]);
    append_frame_buffer(&screen, (const char *)buf);
    if (i++ >= len)
      break;
    sprintf(buf, "%02X", report[i]);
    append_frame_buffer(&screen, (const char *)buf);
  }
  append_frame_buffer(&screen, "\n");
}
