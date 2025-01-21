#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"

#include "app.h"
#include "safe.h"
#include "settings.h"

const AppVTable default_app_vtable = {
    .on_watchdog_reboot = NULL,
    .init = NULL,
    .deinit = NULL,
    .main = NULL,
};

const OutputReport DEFAULTOUTP = {
    .buttons = 0x0000,
    .hat = 0x00,
    .axis_x = 0,
    .axis_y = 0,
    .axis_z = 0,
    .axis_rz = 0,
};

App App_init() {
  return App_initEx(GPIO_LED, AppData_default(), &default_app_vtable);
}

App App_initEx(uint led_pin, AppData app_data, const AppVTable *vtable) {
  // Set the appropriate clock speed for USB
  set_sys_clock_khz(120000, true);

  // Add our led PIN
  gpio_init(led_pin);
  gpio_set_dir(led_pin, GPIO_OUT);
  gpio_put(led_pin, 1);

  // Run our watchdog reboot code if we halted and
  // rebooted from watchdog timeout
  if (watchdog_caused_reboot()) {
    safecall(vtable->on_watchdog_reboot);
  }

  watchdog_enable(WATCHDOG_TIMEOUT, WATCHDOG_DEBUG);

  return (App){
      .led_pin = led_pin,
      .data = app_data,
      .vtable = vtable,
  };
}

void App_run(App *app) {

  safecall(app->vtable->init, &app->data);

  while (true) {
    safecall(app->vtable->main, &app->data);
    watchdog_update();
  }

  safecall(app->vtable->deinit, &app->data);
}

void App_destroy(App *app) { (void)app; }
