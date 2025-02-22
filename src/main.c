#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "display.h"

LOG_MODULE_REGISTER(display);

#define SSD1306_WIDTH      128
#define SSD1306_HEIGHT     64

/* The SSD1306 driver in Zephyr expects 1 byte per column per "page" of 8 rows.
 * For a 128×64 display, we have 8 pages (64/8), each 128 bytes wide = 1024 bytes total.
 */
#define SSD1306_PAGES      (SSD1306_HEIGHT / 8)
#define SSD1306_BUF_SIZE   (SSD1306_WIDTH * SSD1306_PAGES)

/* Frame buffer for page-based addressing */
static uint8_t frame_buffer[SSD1306_BUF_SIZE];

void main(void)
{
    /* Get the SSD1306 display device */
    const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));
    if (!display) {
        LOG_ERR("Display device not found");
        return;
    }

    if (!device_is_ready(display)) {
        LOG_ERR("Display device not ready");
        return;
    }

    /* Prepare the display buffer descriptor */
    struct display_buffer_descriptor buf_desc = {
      .width    = SSD1306_WIDTH,
      .height   = SSD1306_HEIGHT,
      .pitch    = SSD1306_WIDTH,
      .buf_size = sizeof(frame_buffer)
    };

    /* Clear the frame buffer */
    memset(frame_buffer, 0, sizeof(frame_buffer));

    displayString(display, buf_desc, "Hello", 0, 0);
    k_sleep(K_SECONDS(3));
    displayString(display, buf_desc, "World", 0, 10);
    k_sleep(K_SECONDS(3));
    clearDisplay(display, buf_desc);
    
    while (1) {
        k_sleep(K_SECONDS(1));
    }
}

void displayString(const struct device *display, struct display_buffer_descriptor buf_desc, const char *str, int x, int y) {
  draw_string_page_based(frame_buffer, x, y, str, SSD1306_WIDTH);
  int ret = display_write(display, 0, 0, &buf_desc, frame_buffer);
  if (ret < 0) {
      LOG_ERR("Failed to write to display (err %d)", ret);
      return;
  }
}

void clearDisplay(const struct device *display, struct display_buffer_descriptor buf_desc) {
  memset(frame_buffer, 0, sizeof(frame_buffer));
  int ret = display_write(display, 0, 0, &buf_desc, frame_buffer);
  if (ret < 0) {
      LOG_ERR("Failed to write to display (err %d)", ret);
      return;
  }
}
