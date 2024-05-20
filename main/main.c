#include <esp_err.h>
#include <esp_log.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <freertos/FreeRTOS.h>
#pragma GCC diagnostic pop
#include <freertos/task.h>
#include <math.h>
#include <nvs_flash.h>
#include <stdio.h>

#include "display.h"
#include "lcd.h"
#include "lvgl.h"
#include "sdkconfig.h"

static const char *TAG = "main";

static void anim_x_cb(void *var, int32_t v) { lv_obj_set_x(var, v); }

static void anim_size_cb(void *var, int32_t v) { lv_obj_set_size(var, v * 4, v * 4); }

void lvgl_dummy_ui(lv_display_t *disp) {
    lv_obj_t *scr = lv_display_get_screen_active(disp);

    // Text label
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, "Hello Espressif, Hello LVGL. This must be a bit longer to scoll.");
    lv_obj_set_width(label, 240);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // Animation
    lv_obj_t *obj = lv_obj_create(scr);
    lv_color_t red = LV_COLOR_MAKE(255, 0, 0);
    lv_obj_set_style_bg_color(obj, red, 0);
    lv_color_t grn = LV_COLOR_MAKE(0, 255, 0);
    lv_obj_set_style_border_color(obj, grn, 0);
    lv_obj_set_style_border_width(obj, 5, 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 10, 40);
    lv_anim_set_duration(&a, 1000);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_playback_duration(&a, 300);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, anim_size_cb);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_values(&a, 10, 240);
    lv_anim_start(&a);
}

void app_main(void) {
    ESP_LOGI(TAG, "app_main()");

    ESP_LOGI(TAG, "Initialize LCD");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_io_handle_t panel_io_handle = NULL;
    init_lcd(&panel_handle, &panel_io_handle);
    assert(panel_handle != NULL);
    assert(panel_io_handle != NULL);

    ESP_LOGI(TAG, "Initialize display");
    lv_display_t *disp = NULL;
    init_display(panel_handle, panel_io_handle, &disp);
    assert(disp != NULL);

    ESP_LOGI(TAG, "Display LVGL animation");
    lvgl_dummy_ui(disp);

    uint8_t c = 0;
    while (1) {
        uint32_t time_till_next_ms = lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(time_till_next_ms));

        lcd_backlight_set_brightness(c++);
    }
}
