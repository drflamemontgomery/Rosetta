#pragma once

//========================================---
// LED SETTINGS
//========================================---

enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

//========================================---
// USB OUTPUT SETTINGS
//========================================---

#define MAX_OUTPUTS 4

//========================================---
// GPIO SETTINGS
//========================================---

#define GPIO_LED 28

//========================================---
// USB CONNECTIONS
//========================================---

#define MAX_USB_INPUTS 7
#define MAX_USB_DEVICES 8

//========================================---
// USB HOST SETTINGS
//========================================---

#define USB_HOST_PIN_1 2

#define USB_HOST_ADD_PORT 0
#if USB_HOST_ADD_PORT

#define USB_HOST_PIN_2 6

#endif

//========================================---
// WATCHDOG SETTINGS
//========================================---


#define WATCHDOG_TIMEOUT 100
#define WATCHDOG_DEBUG 1
