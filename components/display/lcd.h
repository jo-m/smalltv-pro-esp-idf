#pragma once

#include <esp_lcd_types.h>
#include <stdint.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <freertos/FreeRTOS.h>
#pragma GCC diagnostic pop

#include "lvgl.h"

#define SMALLTV_LCD_H_RES 240
#define SMALLTV_LCD_V_RES 240
#define SMALLTV_LCD_COLOR_FORMAT LV_COLOR_FORMAT_RGB565
#define SMALLTV_LCD_COLOR_DEPTH_BIT 16
#define SMALLTV_LCD_COLOR_DEPTH_BYTE (SMALLTV_LCD_COLOR_DEPTH_BIT / 8)

#define SMALLTV_LCD_SPI_HOST SPI2_HOST
#define SMALLTV_LCD_CMD_BITS 8
#define SMALLTV_LCD_PARAM_BITS 8

typedef struct lcd_t {
    esp_lcd_panel_handle_t panel_handle;
    esp_lcd_panel_io_handle_t panel_io_handle;

    // Those fields are private to the implementation.
    SemaphoreHandle_t _draw_sem;
    StaticSemaphore_t _draw_sem_buf;
    SemaphoreHandle_t _done_sem;
    StaticSemaphore_t _done_sem_buf;
} lcd_t;

void init_lcd(lcd_t *lcd_out);

void lcd_backlight_set_brightness(uint8_t duty);
