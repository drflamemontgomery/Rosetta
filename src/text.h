#ifndef __TEXT_H
#define __TEXT_H
#include "font.h"
#include "ssd1306.h"
#include <stdio.h>
#include <stdlib.h>

#define TEXT_BUFFER_WIDTH (SSD1306_LCDWIDTH / (char_width + 1))
#define TEXT_BUFFER_HEIGHT (SSD1306_LCDHEIGHT / (char_height + 1))
#define TEXT_BUFFER_LEN (TEXT_BUFFER_WIDTH * TEXT_BUFFER_HEIGHT)

typedef struct frame_buffer_struct {
  int len;
  int width;
  int height;

  int x;
  int y;

  bool wrap_text;
  bool scroll;

  int index;

  char *buf;

  void (*append)(struct frame_buffer_struct *, char);
} frame_buffer_t;

void create_frame_buffer(frame_buffer_t *buffer, int width, int height);
void display_frame_buffer(ssd1306_t *display, frame_buffer_t *buffer, int x,
                          int y, int width, int height, bool invert);
void append_frame_buffer(frame_buffer_t *buffer, const char *msg);
void draw_char(ssd1306_t *display, unsigned char code, int16_t x, int16_t y,
               bool invert);

#endif //__TEXT_H
