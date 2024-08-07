#ifndef SSD1306_H
#define SSD1306_H

#define SSD1306_BLACK 0
#define SSD1306_WHITE 1
#define SSD1306_INVERSE 2

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range


#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

#define WIDTH SSD1306_LCDWIDTH
#define HEIGHT SSD1306_LCDHEIGHT

#ifndef __DESKTOP_TESTING
#include "pico/stdlib.h"
#endif

#include <stdbool.h>

typedef struct struct_ssd1306 {
#ifdef __DESKTOP_TESTING
  unsigned char *surface_data;
  cairo_surface_t *surface;
  cairo_surface_t *double_buffer;
  cairo_t *cr;
#else
  int8_t mosi_pin;
  int8_t clk_pin;
  int8_t dc_pin;
  int8_t rst_pin;
  int8_t cs_pin;

  int8_t vccstate;
  int8_t contrast;

  uint8_t *buffer;
#endif
} ssd1306_t;

void ssd1306_init(ssd1306_t *config);

bool ssd1306_begin(ssd1306_t *config, uint8_t vcs, bool reset);
void ssd1306_display(ssd1306_t *config);
void ssd1306_clearDisplay(ssd1306_t *config);
void ssd1306_invertDisplay(ssd1306_t *config, bool i);
void ssd1306_dim(ssd1306_t *config, bool dim);
void ssd1306_drawPixel(ssd1306_t *config, int16_t x, int16_t y, uint16_t color);
void ssd1306_drawFastHLine(ssd1306_t *config, int16_t x, int16_t y, int16_t w, uint16_t color);
void ssd1306_drawFastVLine(ssd1306_t *config, int16_t x, int16_t y, int16_t h, uint16_t color);
void ssd1306_startscrollright(ssd1306_t *config, uint8_t start, uint8_t stop);
void ssd1306_startscrollleft(ssd1306_t *config, uint8_t start, uint8_t stop);
void ssd1306_startscrolldiagright(ssd1306_t *config, uint8_t start, uint8_t stop);
void ssd1306_startscrolldiagleft(ssd1306_t *config, uint8_t start, uint8_t stop);
void ssd1306_stopscroll(ssd1306_t *config);
void ssd1306_command(ssd1306_t *config, uint8_t c);
bool ssd1306_getPixel(ssd1306_t *config, int16_t x, int16_t y);

#endif //SSD1306_H
