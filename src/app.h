#ifndef _APP_H
#define _APP_H

#include "pico/stdio.h"

// Data passed into the App Functions
typedef struct AppData {
  uint led_pin;
} AppData;

// Virtual Functions to overload for the app
typedef struct AppVTable {
  void (*on_watchdog_reboot)(void);

  void (*init)(AppData *);
  void (*deinit)(AppData *);
  void (*main)(AppData *);
} AppVTable;

typedef struct App {
  AppData data;
  const AppVTable *vtable;
} App;

App App_init();
App App_initEx(uint led_pin, const AppVTable *vtable);
void App_run(App *app);
void App_destroy(App *app);

#endif //_APP_H
