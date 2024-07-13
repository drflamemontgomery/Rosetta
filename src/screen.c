#include "settings.h"
#include "ssd1306.h"
#include "text.h"

ssd1306_t display = {
    .mosi_pin = MOSI_PIN,
    .clk_pin = CLK_PIN,
    .dc_pin = DC_PIN,
    .cs_pin = CS_PIN,
    .rst_pin = RST_PIN,
    .buffer = NULL,
};
frame_buffer_t screen;

void screen_init(void) {

  ssd1306_init(&display);
  if (!ssd1306_begin(&display, SSD1306_SWITCHCAPVCC, true)) {
    for (;;)
      sleep_ms(2000);
  }

  ssd1306_display(&display);
  sleep_ms(SPLASH_DISPLAY_TIME);

  create_frame_buffer(&screen, TEXT_BUFFER_WIDTH, TEXT_BUFFER_HEIGHT);
}

void screen_display(void) {
  ssd1306_clearDisplay(&display);
  display_frame_buffer(&display, &screen, 0, 0, screen.width, screen.height,
                       false);
  ssd1306_display(&display);
}
