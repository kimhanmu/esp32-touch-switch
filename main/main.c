/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "ui/ui.h"
#include "waveshare_rgb_lcd_port.h"
#include "esp_log.h"

static const char *TAG_UI = "UI_EVENT";

// 0~9 이미지 배열 (이렇게 해두면 img_nums[3] 하면 3번 이미지가 바로 튀어나옵니다!)
const lv_img_dsc_t * img_nums[10] = {
    &ui_img_numb_0_png, &ui_img_numb_1_png, &ui_img_numb_2_png, &ui_img_numb_3_png, 
    &ui_img_numb_4_png, &ui_img_numb_5_png, &ui_img_numb_6_png, &ui_img_numb_7_png, 
    &ui_img_numb_8_png, &ui_img_numb_9_png
};

// 시계 업데이트 함수 (이제 500ms마다 실행됩니다!)
void clock_update_cb(lv_timer_t * timer)
{
    // 시작 시간 설정 (금방 분이 넘어가는 걸 보려고 11시 58분 55초로 세팅했습니다)
    static int hour = 11;
    static int minute = 58;
    static int second = 55; // 초 단위 추가!
    static bool is_pm = true;

    // 콜론 깜빡임 상태 변수
    static bool colon_show = true;

    // 1. 콜론 깜빡임 로직 (500ms마다 토글)
    colon_show = !colon_show;
    
    // ui_ImgColon 이름은 부장님이 SquareLine에서 만드신 콜론 위젯 이름에 맞추세요!
    if (colon_show) {
        // 불투명도를 255(100%)로 설정 (화면에 보임, 자리는 유지)
        lv_obj_set_style_img_opa(ui_ImgColon, 255, 0);
    } else {
        // 불투명도를 0(0%)으로 설정 (투명해짐, 자리는 유지)
        lv_obj_set_style_img_opa(ui_ImgColon, 0, 0);
    }

    // 2. 정상적인 시간 흐름 (콜론이 켜질 때 = 즉 2번 실행될 때마다 = 1초마다)
    if (colon_show) {
        second++;
        if (second >= 60) {
            second = 0;
            minute++;
            if (minute >= 60) {
                minute = 0;
                hour++;
                if (hour == 12) {
                    is_pm = !is_pm; // 12시가 되면 AM/PM 반전
                }
                if (hour > 12) {
                    hour = 1; // 13시 대신 1시
                }
            }
        }
    }

    // 3. 이미지 교체 로직 (기존과 동일)
    if (is_pm) {
        lv_img_set_src(ui_ImgAMPM, &ui_img_pm_png);
    } else {
        lv_img_set_src(ui_ImgAMPM, &ui_img_am_png);
    }

    lv_img_set_src(ui_ImgHour10, img_nums[hour / 10]);
    lv_img_set_src(ui_ImgHour1,  img_nums[hour % 10]);
    lv_img_set_src(ui_ImgMin10, img_nums[minute / 10]);
    lv_img_set_src(ui_ImgMin1,  img_nums[minute % 10]);
}

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
        // 기존 1000을 500으로 변경! (0.5초마다 콜백 함수 실행)
           lv_timer_create(clock_update_cb, 500, NULL);
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
