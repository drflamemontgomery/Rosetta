#include "app.h"
#include "hardware/gpio.h"
#include "settings.h"
#include "tusb.h"

void rosetta_init(AppData *data);
void rosetta_main(AppData *data);
void debug_reboot(void);
void screen_init(void);    // screen.c
void screen_display(void); // screen.c

const AppVTable rosetta_app_vtable = {
    .on_watchdog_reboot = debug_reboot,
    .init = rosetta_init,
    .main = rosetta_main,
    .deinit = NULL,
};

int main(void) {

  App app = App_initEx(GPIO_LED, AppData_default(), &rosetta_app_vtable);
  App_run(&app);
  App_destroy(&app);

  return 0;
}

void rosetta_init(AppData *data) {
  (void)data;

  screen_init();
  tud_init(0);
}

void rosetta_main(AppData *data) {
  (void)data;
  screen_display();
  tud_task();

  const uint32_t interval_us = 1000;
  static uint32_t start_us = 0;

  if (time_us_64() - start_us < interval_us)
    return;
  start_us += interval_us;

  for (int i = 0; i < data->outputs.len; i++) {
    UsbOutput_sendReport(&data->outputs.ptr[i]);
  }
}

void debug_reboot(void) { gpio_put(GPIO_LED, 0); }
