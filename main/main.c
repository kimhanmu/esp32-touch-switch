/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "ui/ui.h"
#include "waveshare_rgb_lcd_port.h"
#include "esp_log.h"

static const char *TAG_UI = "UI_EVENT";

void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 
    // wavesahre_rgb_lcd_bl_on();  //Turn on the screen backlight 
    // wavesahre_rgb_lcd_bl_off(); //Turn off the screen backlight 
    
    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1)) {
        // lv_demo_stress();
        // lv_demo_benchmark();
        // lv_demo_music();
        // lv_demo_widgets();
        // example_lvgl_demo_ui();
        // 드디어 도화지에 버튼을 그립니다!
           ui_init();
        // Release the mutex
        lvgl_port_unlock();
    }
}

void ui_event_light1_switch(lv_event_t * e) 
{
    lv_obj_t * obj = lv_event_get_target(e);
    
    // 1. 현재 발생한 입력(터치) 디바이스 정보 가져오기
    lv_indev_t * indev = lv_indev_get_act();
    if(indev != NULL) {
        lv_point_t pt;
        lv_indev_get_point(indev, &pt); // 2. 터치된 X, Y 좌표 추출
        
        // 3. 터미널에 좌표 출력
        ESP_LOGI(TAG_UI, "🎯 Touch Coordinates -> X: %d, Y: %d", pt.x, pt.y);
    }

    // 기존 스위치 ON/OFF 로그
    if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        ESP_LOGI(TAG_UI, ">>> Light 1: [ON]"); 
    } else {
        ESP_LOGI(TAG_UI, ">>> Light 1: [OFF]");
    }
}

void ui_event_light2_switch(lv_event_t * e) 
{
    lv_obj_t * obj = lv_event_get_target(e);
    
    // 1. 현재 발생한 입력(터치) 디바이스 정보 가져오기
    lv_indev_t * indev = lv_indev_get_act();
    if(indev != NULL) {
        lv_point_t pt;
        lv_indev_get_point(indev, &pt); // 2. 터치된 X, Y 좌표 추출
        
        // 3. 터미널에 좌표 출력
        ESP_LOGI(TAG_UI, "🎯 Touch Coordinates -> X: %d, Y: %d", pt.x, pt.y);
    }

    // 기존 스위치 ON/OFF 로그
    if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        ESP_LOGI(TAG_UI, ">>> Light 2: [ON]"); 
    } else {
        ESP_LOGI(TAG_UI, ">>> Light 2: [OFF]");
    }
}

void ui_event_Boiler_switch(lv_event_t * e) 
{
    lv_obj_t * obj = lv_event_get_target(e);
    
    // 1. 현재 발생한 입력(터치) 디바이스 정보 가져오기
    lv_indev_t * indev = lv_indev_get_act();
    if(indev != NULL) {
        lv_point_t pt;
        lv_indev_get_point(indev, &pt); // 2. 터치된 X, Y 좌표 추출
        
        // 3. 터미널에 좌표 출력
        ESP_LOGI(TAG_UI, "🎯 Touch Coordinates -> X: %d, Y: %d", pt.x, pt.y);
    }

    // 기존 스위치 ON/OFF 로그
    if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
        ESP_LOGI(TAG_UI, ">>> Boiler : [ON]"); 
    } else {
        ESP_LOGI(TAG_UI, ">>> Boiler : [OFF]");
    }
}

void scroll_to_page1(lv_event_t * e)
{
	// Your code here
	// ⭐️ page1이 화면 중앙에 오도록 부드럽게(ANIM_ON) 스크롤해라!
    lv_obj_scroll_to_view(ui_page1, LV_ANIM_ON);
}

void scroll_to_page2(lv_event_t * e)
{
	// Your code here
	// ⭐️ page2가 화면 중앙에 오도록 부드럽게(ANIM_ON) 스크롤해라!
    lv_obj_scroll_to_view(ui_page2, LV_ANIM_ON);
}
