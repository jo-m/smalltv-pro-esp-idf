#pragma once

#include <esp_err.h>
#include <esp_lcd_types.h>

#include "lvgl.h"

esp_err_t display_init(esp_lcd_panel_handle_t panel_handle,
                       esp_lcd_panel_io_handle_t panel_io_handle, lv_display_t **disp_out);
