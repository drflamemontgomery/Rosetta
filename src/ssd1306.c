#include <stdlib.h>
#include <string.h>

#include "splash.h"
#include "ssd1306.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"

uint8_t rotation = 0;
uint8_t ssd1306_getRotation() {
  return rotation;
}

void ssd1306_drawFastHLineInternal(ssd1306_t *config, int16_t x, int16_t y, int16_t w, uint16_t color);
void ssd1306_drawFastVLineInternal(ssd1306_t *config, int16_t x, int16_t y, int16_t h, uint16_t color);
void ssd1306_command1(ssd1306_t *config, uint8_t c);
void ssd1306_commandList(ssd1306_t *config, const uint8_t *c, uint8_t n);
void ssd1306_drawBitmap(ssd1306_t *config, int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);


/*#define SSD1306_SELECT gpio_put(config->cs_pin, false);
#define SSD1306_DESELECT gpio_put(config->cs_pin, true);
#define SSD1306_MODE_COMMAND gpio_put(config->dc_pin, false);
#define SSD1306_MODE_DATA gpio_put(config->dc_pin, true);

void spi_write(ssd1306_t *config, uint8_t byte) {
  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    gpio_put(config->mosi_pin, byte & bit);
    gpio_put(config->clk_pin, 1);
    gpio_put(config->clk_pin, 0);
  }
}*/

void ssd1306_init(ssd1306_t *config) {
  gpio_init(config->mosi_pin);
  gpio_init(config->clk_pin);
  gpio_init(config->cs_pin);
  gpio_init(config->rst_pin);
  gpio_init(config->dc_pin);
}

/*void command1_ssd1306(ssd1306_t *config, uint8_t byte) {
  SSD1306_MODE_COMMAND
  spi_write(config, byte);
}

void commandList_ssd1306(ssd1306_t *config, const uint8_t *buf, uint8_t len) {
  SSD1306_MODE_COMMAND
  //spi_write_blocking(config->spi, buf, len);
  while(len--) {
    spi_write(config, *buf++);
  }
}

void spi_write_byte_blocking(spi_inst_t *inst, uint8_t byte) {
  spi_write_blocking(inst, &byte, 1);
}

bool begin_ssd1306(ssd1306_t *config) {
  if(!config->buffer && !(config->buffer = malloc(SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8)))) return false;
  clear_display_ssd1306(config);

  gpio_set_dir(config->dc_pin, GPIO_OUT);
  gpio_set_dir(config->cs_pin, GPIO_OUT);
  
  SSD1306_DESELECT
  gpio_set_dir(config->mosi_pin, GPIO_OUT);
  gpio_set_dir(config->clk_pin, GPIO_OUT);

  gpio_put(config->clk_pin, 0);
  *gpio_set_dir(config->mosi_pin, GPIO_OUT);
  gpio_set_dir(config->clk_pin, GPIO_OUT);

  gpio_put(config->clk_pin, 0);*

  SSD1306_SELECT
  
  static const uint8_t init1[] = {
    SSD1306_DISPLAYOFF,
    SSD1306_SETDISPLAYCLOCKDIV,
    0x80,
    SSD1306_SETMULTIPLEX
  };

  commandList_ssd1306(config, init1, sizeof(init1));
  command1_ssd1306(config, 63);
  //spi_write_blocking(config->spi, init1, sizeof(init1));
  //spi_write_byte_blocking(config->spi, 63);
  
  static const uint8_t init2[] = {
    SSD1306_SETDISPLAYOFFSET,
    0x0,
    SSD1306_SETSTARTLINE | 0x0,
    SSD1306_CHARGEPUMP
  };

  commandList_ssd1306(config, init2, sizeof(init2));
  command1_ssd1306(config, 0x14);
  //spi_write_blocking(config->spi, init2, sizeof(init2));
  //spi_write_byte_blocking(config->spi, 0x14);

  static const uint8_t init3[] = {
    SSD1306_MEMORYMODE,
    0x00,
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC
  };

  commandList_ssd1306(config, init3, sizeof(init3));
  //spi_write_blocking(config->spi, init3, sizeof(init3));

  uint8_t com_pins = 0x12;
  uint8_t config->contrast = 0xCF;
  
  command1_ssd1306(config, SSD1306_SETCOMPINS);
  command1_ssd1306(config, com_pins);
  command1_ssd1306(config, SSD1306_SETCONTRAST);
  command1_ssd1306(config, config->contrast);
  //spi_write_byte_blocking(config->spi, SSD1306_SETCOMPINS);
  //spi_write_byte_blocking(config->spi, com_pins);
  //spi_write_byte_blocking(config->spi, SSD1306_SETCONTRAST);
  //spi_write_byte_blocking(config->spi, config->contrast);

  command1_ssd1306(config, SSD1306_SETPRECHARGE);
  command1_ssd1306(config, 0xF1);
  //spi_write_byte_blocking(config->spi, SSD1306_SETPRECHARGE);
  //spi_write_byte_blocking(config->spi, 0xF1);

  static const uint8_t init5[] = {
    SSD1306_SETVCOMDETECT,
    0x40,
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_NORMALDISPLAY,
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON
  };

  commandList_ssd1306(config, init5, sizeof(init5));
  //spi_write_blocking(config->spi, init5, sizeof(init5));

  SSD1306_DESELECT

  return true;
}

void clear_display_ssd1306(ssd1306_t *config) {
  memset(config->buffer, 0, SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8));
}

void display_ssd1306(ssd1306_t *config) {
  SSD1306_SELECT
  static const uint8_t dlist1[] = {
    SSD1306_PAGEADDR,
    0,
    0xFF,
    SSD1306_COLUMNADDR, 0
  };

  commandList_ssd1306(config, dlist1, sizeof(dlist1));
  command1_ssd1306(config, SSD1306_LCDWIDTH - 1);
  //spi_write_blocking(config->spi, dlist1, sizeof(dlist1));
  //spi_write_byte_blocking(config->spi, SSD1306_LCDWIDTH - 1);

  uint16_t count = SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8);
  uint8_t *ptr = config->buffer;

  SSD1306_MODE_DATA
  while(count--) {
    spi_write(config, *ptr++);
  }
  //spi_write_blocking(config->spi, config->buffer, count);

  SSD1306_DESELECT
}*/



#define pgm_read_byte(addr)                                                    \
  (*(const unsigned char *)(addr)) ///< workaround for non-AVR

// SOME DEFINES AND STATIC VARIABLES USED INTERNALLY -----------------------
#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

#define SSD1306_SELECT gpio_put(config->cs_pin, false);       ///< Device select
#define SSD1306_DESELECT gpio_put(config->cs_pin, true);    ///< Device deselect
#define SSD1306_MODE_COMMAND gpio_put(config->dc_pin, false); ///< Command mode
#define SSD1306_MODE_DATA gpio_put(config->dc_pin, true);   ///< Data mode

#define SPI_TRANSACTION_START ///< Dummy stand-in define
#define SPI_TRANSACTION_END   ///< keeps compiler happy

// Check first if Wire, then hardware SPI, then soft SPI:
#define TRANSACTION_START                                                      \
    SSD1306_SELECT;
#define TRANSACTION_END                                                        \
    SSD1306_DESELECT;

static inline void ssd1306_write(ssd1306_t *config, uint8_t d) {
  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    gpio_put(config->mosi_pin, d & bit);
    gpio_put(config->clk_pin, 1);
    gpio_put(config->clk_pin, 0);
  }
}

void ssd1306_command1(ssd1306_t *config, uint8_t c) {
  SSD1306_MODE_COMMAND
  ssd1306_write(config, c);
}

void ssd1306_commandList(ssd1306_t *config, const uint8_t *c, uint8_t n) {
  SSD1306_MODE_COMMAND
  while (n--)
    ssd1306_write(config, pgm_read_byte(c++));
}

void ssd1306_command(ssd1306_t *config, uint8_t c) {
  TRANSACTION_START
  ssd1306_command1(config, c);
  TRANSACTION_END
}

bool ssd1306_begin(ssd1306_t *config, uint8_t vcs, bool reset) {

  if ((!config->buffer) && !(config->buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;

  ssd1306_clearDisplay(config);
  config->vccstate = vcs;

  // Setup pin directions

  gpio_set_dir(config->dc_pin, true);
  gpio_set_dir(config->cs_pin, true);
  SSD1306_DESELECT

  gpio_set_dir(config->mosi_pin, true);
  gpio_set_dir(config->clk_pin, true);
  gpio_put(config->clk_pin, false); // Clock low

  // Reset SSD1306 if requested and reset pin specified in constructor
  if (reset && (config->rst_pin >= 0)) {
    gpio_set_dir(config->rst_pin, true);
    gpio_put(config->rst_pin, true);
    sleep_ms(1);                   // VDD goes high at start, pause for 1 ms
    gpio_put(config->rst_pin, false);  // Bring reset low
    sleep_ms(10);                  // Wait 10 ms
    gpio_put(config->rst_pin, true); // Bring out of reset
  }

  TRANSACTION_START

  // Init sequence
  static const uint8_t init1[] = {SSD1306_DISPLAYOFF,         // 0xAE
                                          SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
                                          0x80, // the suggested ratio 0x80
                                          SSD1306_SETMULTIPLEX}; // 0xA8
  ssd1306_commandList(config, init1, sizeof(init1));
  ssd1306_command1(config, HEIGHT - 1);

  static const uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
                                          0x0,                      // no offset
                                          SSD1306_SETSTARTLINE | 0x0, // line #0
                                          SSD1306_CHARGEPUMP};        // 0x8D
  ssd1306_commandList(config, init2, sizeof(init2));

  ssd1306_command1(config, (config->vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

  static const uint8_t init3[] = {SSD1306_MEMORYMODE, // 0x20
                                          0x00, // 0x0 act like ks0108
                                          SSD1306_SEGREMAP | 0x1,
                                          SSD1306_COMSCANDEC};
  ssd1306_commandList(config, init3, sizeof(init3));

  uint8_t com_pins = 0x02;
  config->contrast = 0x8F;

  if ((WIDTH == 128) && (HEIGHT == 32)) {
    com_pins = 0x02;
    config->contrast = 0x8F;
  } else if ((WIDTH == 128) && (HEIGHT == 64)) {
    com_pins = 0x12;
    config->contrast = (config->vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
  } else if ((WIDTH == 96) && (HEIGHT == 16)) {
    com_pins = 0x2; // ada x12
    config->contrast = (config->vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
  } else {
    // Other screen varieties -- TBD
  }

  ssd1306_command1(config, SSD1306_SETCOMPINS);
  ssd1306_command1(config, com_pins);
  ssd1306_command1(config, SSD1306_SETCONTRAST);
  ssd1306_command1(config, config->contrast);

  ssd1306_command1(config, SSD1306_SETPRECHARGE); // 0xd9
  ssd1306_command1(config, (config->vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
  static const uint8_t init5[] = {
      SSD1306_SETVCOMDETECT, // 0xDB
      0x40,
      SSD1306_DISPLAYALLON_RESUME, // 0xA4
      SSD1306_NORMALDISPLAY,       // 0xA6
      SSD1306_DEACTIVATE_SCROLL,
      SSD1306_DISPLAYON}; // Main screen turn on
  ssd1306_commandList(config, init5, sizeof(init5));

  TRANSACTION_END

  ssd1306_clearDisplay(config);
  ssd1306_drawBitmap(config, 41, 0, splash_screen, splash_width, splash_height, SSD1306_WHITE);
  ssd1306_invertDisplay(config, true);

  return true; // Success
}

void ssd1306_drawPixel(ssd1306_t *config, int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (ssd1306_getRotation()) {
    case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    switch (color) {
    case SSD1306_WHITE:
      config->buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
      break;
    case SSD1306_BLACK:
      config->buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
      break;
    case SSD1306_INVERSE:
      config->buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
      break;
    }
  }
}

void ssd1306_clearDisplay(ssd1306_t *config) {
  memset(config->buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}

void ssd1306_drawFastHLine(ssd1306_t *config, int16_t x, int16_t y, int16_t w,
                                     uint16_t color) {
  bool bSwap = false;
  switch (rotation) {
  case 1:
    // 90 degree rotation, swap x & y for rotation, then invert x
    bSwap = true;
    ssd1306_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    x -= (w - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation,
    // then invert y and adjust y for w (not to become h)
    bSwap = true;
    ssd1306_swap(x, y);
    y = HEIGHT - y - 1;
    y -= (w - 1);
    break;
  }

  if (bSwap)
    ssd1306_drawFastVLineInternal(config, x, y, w, color);
  else
    ssd1306_drawFastHLineInternal(config, x, y, w, color);
}

void ssd1306_drawFastHLineInternal(ssd1306_t *config, int16_t x, int16_t y, int16_t w,
                                             uint16_t color) {

  if ((y >= 0) && (y < HEIGHT)) { // Y coord in bounds?
    if (x < 0) {                  // Clip left
      w += x;
      x = 0;
    }
    if ((x + w) > WIDTH) { // Clip right
      w = (WIDTH - x);
    }
    if (w > 0) { // Proceed only if width is positive
      uint8_t *pBuf = &config->buffer[(y / 8) * WIDTH + x], mask = 1 << (y & 7);
      switch (color) {
      case SSD1306_WHITE:
        while (w--) {
          *pBuf++ |= mask;
        };
        break;
      case SSD1306_BLACK:
        mask = ~mask;
        while (w--) {
          *pBuf++ &= mask;
        };
        break;
      case SSD1306_INVERSE:
        while (w--) {
          *pBuf++ ^= mask;
        };
        break;
      }
    }
  }
}

void ssd1306_drawFastVLine(ssd1306_t *config, int16_t x, int16_t y, int16_t h,
                                     uint16_t color) {
  bool bSwap = false;
  switch (rotation) {
  case 1:
    // 90 degree rotation, swap x & y for rotation,
    // then invert x and adjust x for h (now to become w)
    bSwap = true;
    ssd1306_swap(x, y);
    x = WIDTH - x - 1;
    x -= (h - 1);
    break;
  case 2:
    // 180 degree rotation, invert x and y, then shift y around for height.
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    y -= (h - 1);
    break;
  case 3:
    // 270 degree rotation, swap x & y for rotation, then invert y
    bSwap = true;
    ssd1306_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  if (bSwap)
    ssd1306_drawFastHLineInternal(config, x, y, h, color);
  else
    ssd1306_drawFastVLineInternal(config, x, y, h, color);
}

void ssd1306_drawFastVLineInternal(ssd1306_t *config, int16_t x, int16_t __y,
                                             int16_t __h, uint16_t color) {

  if ((x >= 0) && (x < WIDTH)) { // X coord in bounds?
    if (__y < 0) {               // Clip top
      __h += __y;
      __y = 0;
    }
    if ((__y + __h) > HEIGHT) { // Clip bottom
      __h = (HEIGHT - __y);
    }
    if (__h > 0) { // Proceed only if height is now positive
      // this display doesn't need ints for coordinates,
      // use local byte registers for faster juggling
      uint8_t y = __y, h = __h;
      uint8_t *pBuf = &config->buffer[(y / 8) * WIDTH + x];

      // do the first partial byte, if necessary - this requires some masking
      uint8_t mod = (y & 7);
      if (mod) {
        // mask off the high n bits we want to set
        mod = 8 - mod;
        // note - lookup table results in a nearly 10% performance
        // improvement in fill* functions
        // uint8_t mask = ~(0xFF >> mod);
        static const uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0,
                                                   0xF0, 0xF8, 0xFC, 0xFE};
        uint8_t mask = pgm_read_byte(&premask[mod]);
        // adjust the mask if we're not going to reach the end of this byte
        if (h < mod)
          mask &= (0XFF >> (mod - h));

        switch (color) {
        case SSD1306_WHITE:
          *pBuf |= mask;
          break;
        case SSD1306_BLACK:
          *pBuf &= ~mask;
          break;
        case SSD1306_INVERSE:
          *pBuf ^= mask;
          break;
        }
        pBuf += WIDTH;
      }

      if (h >= mod) { // More to go?
        h -= mod;
        // Write solid bytes while we can - effectively 8 rows at a time
        if (h >= 8) {
          if (color == SSD1306_INVERSE) {
            // separate copy of the code so we don't impact performance of
            // black/white write version with an extra comparison per loop
            do {
              *pBuf ^= 0xFF; // Invert byte
              pBuf += WIDTH; // Advance pointer 8 rows
              h -= 8;        // Subtract 8 rows from height
            } while (h >= 8);
          } else {
            // store a local value to work with
            uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
            do {
              *pBuf = val;   // Set byte
              pBuf += WIDTH; // Advance pointer 8 rows
              h -= 8;        // Subtract 8 rows from height
            } while (h >= 8);
          }
        }

        if (h) { // Do the final partial byte, if necessary
          mod = h & 7;
          // this time we want to mask the low bits of the byte,
          // vs the high bits we did above
          // uint8_t mask = (1 << mod) - 1;
          // note - lookup table results in a nearly 10% performance
          // improvement in fill* functions
          static const uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07,
                                                      0x0F, 0x1F, 0x3F, 0x7F};
          uint8_t mask = pgm_read_byte(&postmask[mod]);
          switch (color) {
          case SSD1306_WHITE:
            *pBuf |= mask;
            break;
          case SSD1306_BLACK:
            *pBuf &= ~mask;
            break;
          case SSD1306_INVERSE:
            *pBuf ^= mask;
            break;
          }
        }
      }
    } // endif positive height
  }   // endif x in bounds
}

bool ssd1306_getPixel(ssd1306_t *config, int16_t x, int16_t y) {
  if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (ssd1306_getRotation()) {
    case 1:
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    return (config->buffer[x + (y / 8) * WIDTH] & (1 << (y & 7)));
  }
  return false; // Pixel out of bounds
}

// REFRESH DISPLAY ---------------------------------------------------------

void ssd1306_display(ssd1306_t *config) {
  TRANSACTION_START
  static const uint8_t dlist1[] = {
      SSD1306_PAGEADDR,
      0,                      // Page start address
      0xFF,                   // Page end (not really, but works here)
      SSD1306_COLUMNADDR, 0}; // Column start address
  ssd1306_commandList(config, dlist1, sizeof(dlist1));
  ssd1306_command1(config, WIDTH - 1); // Column end address

  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = config->buffer;
  SSD1306_MODE_DATA
  while (count--)
    ssd1306_write(config, *ptr++);
  TRANSACTION_END
}

void ssd1306_startscrollright(ssd1306_t *config, uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t scrollList1a[] = {
      SSD1306_RIGHT_HORIZONTAL_SCROLL, 0X00};
  ssd1306_commandList(config, scrollList1a, sizeof(scrollList1a));
  ssd1306_command1(config, start);
  ssd1306_command1(config, 0X00);
  ssd1306_command1(config, stop);
  static const uint8_t scrollList1b[] = {0X00, 0XFF,
                                                 SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(config, scrollList1b, sizeof(scrollList1b));
  TRANSACTION_END
}

void ssd1306_startscrollleft(ssd1306_t *config, uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t scrollList2a[] = {SSD1306_LEFT_HORIZONTAL_SCROLL,
                                                 0X00};
  ssd1306_commandList(config, scrollList2a, sizeof(scrollList2a));
  ssd1306_command1(config, start);
  ssd1306_command1(config, 0X00);
  ssd1306_command1(config, stop);
  static const uint8_t scrollList2b[] = {0X00, 0XFF,
                                                 SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(config, scrollList2b, sizeof(scrollList2b));
  TRANSACTION_END
}

void ssd1306_startscrolldiagright(ssd1306_t *config, uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t scrollList3a[] = {
      SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00};
  ssd1306_commandList(config, scrollList3a, sizeof(scrollList3a));
  ssd1306_command1(config, HEIGHT);
  static const uint8_t scrollList3b[] = {
      SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL, 0X00};
  ssd1306_commandList(config, scrollList3b, sizeof(scrollList3b));
  ssd1306_command1(config, start);
  ssd1306_command1(config, 0X00);
  ssd1306_command1(config, stop);
  static const uint8_t scrollList3c[] = {0X01, SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(config, scrollList3c, sizeof(scrollList3c));
  TRANSACTION_END
}

void ssd1306_startscrolldiagleft(ssd1306_t *config, uint8_t start, uint8_t stop) {
  TRANSACTION_START
  static const uint8_t scrollList4a[] = {
      SSD1306_SET_VERTICAL_SCROLL_AREA, 0X00};
  ssd1306_commandList(config, scrollList4a, sizeof(scrollList4a));
  ssd1306_command1(config, HEIGHT);
  static const uint8_t scrollList4b[] = {
      SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0X00};
  ssd1306_commandList(config, scrollList4b, sizeof(scrollList4b));
  ssd1306_command1(config, start);
  ssd1306_command1(config, 0X00);
  ssd1306_command1(config, stop);
  static const uint8_t scrollList4c[] = {0X01, SSD1306_ACTIVATE_SCROLL};
  ssd1306_commandList(config, scrollList4c, sizeof(scrollList4c));
  TRANSACTION_END
}

void ssd1306_stopscroll(ssd1306_t *config) {
  TRANSACTION_START
  ssd1306_command1(config, SSD1306_DEACTIVATE_SCROLL);
  TRANSACTION_END
}

void ssd1306_invertDisplay(ssd1306_t *config, bool i) {
  TRANSACTION_START
  ssd1306_command1(config, i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
  TRANSACTION_END
}

void ssd1306_dim(ssd1306_t *config, bool dim) {
  // the range of config->contrast to too small to be really useful
  // it is useful to dim the display
  TRANSACTION_START
  ssd1306_command1(config, SSD1306_SETCONTRAST);
  ssd1306_command1(config, dim ? 0 : config->contrast);
  TRANSACTION_END
}

void ssd1306_drawBitmap(ssd1306_t *config, int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
  int16_t byteWidth = (w + 7) / 8;
  uint8_t b = 0;

  for(int16_t j = 0; j < h; j++, y++) {
    for(int16_t i = 0; i < w; i++) {
      if(i & 7) b <<= 1;
      else b = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);

      if(b & 0x80) ssd1306_drawPixel(config, x + i, y, color);
    }
  }
}
