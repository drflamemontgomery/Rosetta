#include "text.h"
#include "font.h"
#include <stdlib.h>

void _default_frame_buffer_append(frame_buffer_t *buffer, char c);

void create_frame_buffer(frame_buffer_t *buffer, int width, int height) {
  buffer->len = width * height;

  buffer->width = width;
  buffer->height = height;

  buffer->x = 0;
  buffer->y = 0;

  buffer->index = 0;

  buffer->scroll = true;

  buffer->buf = (char *)calloc(buffer->len, sizeof(char));
  buffer->append = _default_frame_buffer_append;
}

void _default_frame_buffer_append(frame_buffer_t *buffer, char c) {

  if (buffer->index >= buffer->len) {
    if (!buffer->scroll)
      return;
    for (int i = 0; i < buffer->len - buffer->width; i++) {
      buffer->buf[i] = buffer->buf[i + buffer->width];
    }
    for (int i = buffer->len - buffer->width; i < buffer->len; i++) {
      buffer->buf[i] = 0;
    }

    buffer->index -= buffer->width;
  }
  buffer->buf[buffer->index] = c;
  buffer->index += 1;

  if (c == '\n') {
    int index = buffer->index;
    buffer->index += buffer->width;
    buffer->index -= (buffer->index % buffer->width);
    if (buffer->index > buffer->len)
      buffer->index = buffer->len;

    while (index < buffer->index) {
      buffer->buf[index] = 0;
      index += 1;
    }
  }
}

void append_frame_buffer(frame_buffer_t *buffer, const char *msg) {
  if (buffer->append == NULL)
    return;
  for (char *c = (char *)msg; *c != (char)0; c++) {
    buffer->append(buffer, *c);
  }
}

void display_frame_buffer(ssd1306_t *display, frame_buffer_t *buffer, int x,
                          int y, int width, int height, bool invert) {
  for (int ry = 0; ry < (height * (char_height + 1) - 1); ry++) {
    for (int rx = 0; rx < (width * (char_width + 1) - 1); rx++) {
      ssd1306_drawPixel(display, x + rx, y + ry, invert ? 1 : 0);
    }
  }

  int startIndex = buffer->x * buffer->width + buffer->y * buffer->width;
  if ((buffer->width - buffer->x) < width)
    width = buffer->width - buffer->x;

  for (int i = 0; i < width * height; i++) {
    // if(i + startIndex >= buffer->index) return;
    if (i + startIndex >= buffer->len)
      return;

    int tx = (i % width);
    int ty = ((int)(i / width));

    int sx = x + tx * (char_width + 1);
    int sy = y + ty * (char_height + 1);

    int index = startIndex + tx + ty * buffer->width;
    char code = buffer->buf[index];
    if (code == '\n') {
      i += width;
      i -= i % width + 1;
      continue;
    }

    draw_char(display, code, sx, sy, invert);
  }
}

void draw_char(ssd1306_t *display, unsigned char code, int16_t x, int16_t y,
               bool invert) {
  int index = charCode[code] * char_height;
  for (int h = 0; h < char_height; h++) {
    for (int w = 0; w < char_width; w++) {
      int cindex = ((int)(index)) + h;
      int color = (font_data[cindex] >> (char_width - 1 - w)) & 1;
      ssd1306_drawPixel(display, x + w, y + h, invert ? (1 - color) : color);
    }
  }
}
