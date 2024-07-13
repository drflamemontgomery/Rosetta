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

#define GPIO_LED 25 // 28

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

//========================================---
// SSD1306 SCREEN
//========================================---

#define MOSI_PIN 19
#define CLK_PIN 18
#define DC_PIN 16
#define CS_PIN 17
#define RST_PIN 21 // 1

#define SPLASH_DISPLAY_TIME 2000

//========================================---
// FILE SYSTEM SETTINGS
//========================================---

#include "hardware/flash.h"

// File system starts 32 kibibytes before the end of the flash

#define FS_SIZE 0x8000
#define FS_START (XIP_BASE + PICO_FLASH_SIZE_BYTES - FS_SIZE)
#define FLASH_FS_START (PICO_FLASH_SIZE_BYTES - FS_SIZE)
