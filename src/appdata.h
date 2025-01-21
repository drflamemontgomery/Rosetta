#ifndef _APP_DATA_H
#define _APP_DATA_H

#include "usb.h"

typedef struct AppData {
  UsbOutput outputs[4];
} AppData;

AppData AppData_default();

#endif //_APP_DATA_H
