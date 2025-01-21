#ifndef _APP_DATA_H
#define _APP_DATA_H

#include "usb.h"

typedef struct AppData {
  /**
   * USB Output in the format of a list for future compatability with dynamic
   * lists for multiple devices
   */
  struct {
    UsbOutput ptr[4];
    int len;
  } outputs;
} AppData;

AppData AppData_default();

#endif //_APP_DATA_H
