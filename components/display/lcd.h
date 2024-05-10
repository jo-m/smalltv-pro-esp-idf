#pragma once

#include <esp_err.h>
#include <esp_lcd_types.h>
#include <stdint.h>

#include "lvgl.h"

#define SMALLTV_LCD_H_RES 240
#define SMALLTV_LCD_V_RES 240
#define SMALLTV_LCD_COLOR_FORMAT LV_COLOR_FORMAT_RGB565
#define SMALLTV_LCD_COLOR_DEPTH_BIT 16
#define SMALLTV_LCD_COLOR_DEPTH_BYTE (SMALLTV_LCD_COLOR_DEPTH_BIT / 8)

#define SMALLTV_LCD_SPI_HOST SPI2_HOST
#define SMALLTV_LCD_CMD_BITS 8
#define SMALLTV_LCD_PARAM_BITS 8

esp_err_t lcd_init(esp_lcd_panel_handle_t *panel_handle_out,
                   esp_lcd_panel_io_handle_t *panel_io_handle_out);
esp_err_t backlight_set_brightness(uint8_t duty);
