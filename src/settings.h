#pragma once


//========================================---
// GPIO SETTINGS
//========================================---

#define GPIO_LED 25

//========================================---
// USB CONNECTIONS
//========================================---

#define MAX_USB_INPUTS 7

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
