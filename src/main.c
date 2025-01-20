#include "app.h"
#include "settings.h"
#include "tusb.h"

void rosetta_init(AppData *data);
void rosetta_main(AppData *data);

const AppVTable rosetta_app_vtable = {
    .on_watchdog_reboot = NULL,
    .init = rosetta_init,
    .main = rosetta_main,
    .deinit = NULL,
};

int main(void) {

  App app = App_initEx(GPIO_LED, &rosetta_app_vtable);
  App_run(&app);
  App_destroy(&app);

  return 0;
}

void rosetta_init(AppData *data) {
  (void)data;
  tud_init(0);
}

void rosetta_main(AppData *data) {
  (void)data;
  tud_task();
}
