#include "lcd.h"

#include <driver/gpio.h>
#include <esp_lcd_panel_io.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <driver/spi_master.h>
#pragma GCC diagnostic pop
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_log.h>
#include <stdbool.h>

static const char *TAG = "lcd";

esp_err_t lcd_init(esp_lcd_panel_handle_t *panel_handle_out) {
    ESP_LOGI(TAG, "Configure LCD backlight");
    gpio_config_t bk_gpio_config = {.mode = GPIO_MODE_OUTPUT,
                                    .pin_bit_mask = 1ULL << CONFIG_SMALLTV_LCD_BL_PIN};
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    backlight_onoff(false);

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = CONFIG_SMALLTV_LCD_SPI_SCLK_PIN,
        .mosi_io_num = CONFIG_SMALLTV_LCD_SPI_MOSI_PIN,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = CONFIG_SMALLTV_LCD_MAX_TRANSFER_LINES * SMALLTV_LCD_H_RES *
                           SMALLTV_LCD_COLOR_DEPTH_BYTE,
        .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_GPIO_PINS,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SMALLTV_LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Attach LCD panel to SPI bus");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = CONFIG_SMALLTV_LCD_SPI_DC_PIN,
        .cs_gpio_num = -1,  // Not connected, tied to GND
        .pclk_hz = CONFIG_SMALLTV_LCD_PX_CLK_MHZ * 1000000,
        .lcd_cmd_bits = SMALLTV_LCD_CMD_BITS,
        .lcd_param_bits = SMALLTV_LCD_PARAM_BITS,
        .spi_mode = 3,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SMALLTV_LCD_SPI_HOST,
                                             &io_config, &io_handle));

    ESP_LOGI(TAG, "Create St7789 LCD panel");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = CONFIG_SMALLTV_LCD_RST_PIN,
        .rgb_ele_order = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = SMALLTV_LCD_COLOR_DEPTH_BIT,
    };
    esp_lcd_panel_handle_t panel_handle;
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_LOGI(TAG, "Initialize LCD panel");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, 0, 0));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));

    backlight_onoff(true);

    assert(panel_handle_out != NULL);
    *panel_handle_out = panel_handle;
    return ESP_OK;
}

esp_err_t backlight_onoff(bool enable) {
    // TODO: use SMALLTV_LCD_BL_PWM_CHANNEL to dim.

    ESP_LOGI(TAG, "Backlight onoff: %d", enable);

    if (enable) {
        ESP_ERROR_CHECK(gpio_set_level(CONFIG_SMALLTV_LCD_BL_PIN, CONFIG_SMALLTV_LCD_BL_ON_LEVEL));
    } else {
        ESP_ERROR_CHECK(gpio_set_level(CONFIG_SMALLTV_LCD_BL_PIN, CONFIG_SMALLTV_LCD_BL_OFF_LEVEL));
    }

    return ESP_OK;
}
