#pragma once

#include "hardware/flash.h"

// File system starts 32 kibibytes before the end of the flash

#define FS_SIZE        0x8000
#define FS_START       (XIP_BASE + PICO_FLASH_SIZE_BYTES - FS_SIZE)
#define FLASH_FS_START (PICO_FLASH_SIZE_BYTES - FS_SIZE)


