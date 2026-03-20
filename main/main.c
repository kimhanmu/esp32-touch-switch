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

// ==========================================
// 1. 모든 메뉴바 불을 끄는(회색) 헬퍼 함수
// ==========================================
void reset_nav_buttons() {
    lv_img_set_src(ui_navlightclick, &ui_img_light_off_png);
    lv_img_set_src(ui_navbrightclick, &ui_img_bright_off_png);
    lv_img_set_src(ui_navheatclick, &ui_img_heat_off_png);
    lv_img_set_src(ui_navelectricclick, &ui_img_electric_off_png);
    lv_img_set_src(ui_navsettingclick, &ui_img_setting_off_png);
}

// 현재 페이지 번호(0~4)를 받아서 해당 버튼만 파란색/노란색으로 켜는 함수
void update_nav_status(int page_index) {
    reset_nav_buttons(); // 일단 5개 다 끄기 (회색으로 덮기)
    
    // 현재 페이지 번호에 맞춰서 하나만 켜기
    switch(page_index) {
        case 0: lv_img_set_src(ui_navlightclick, &ui_img_light_on_png); break;       // 1페이지: 조명
        case 1: lv_img_set_src(ui_navbrightclick, &ui_img_bright_on_png); break;     // 2페이지: 밝기
        case 2: lv_img_set_src(ui_navheatclick, &ui_img_heat_on_png); break;         // 3페이지: 난방
        case 3: lv_img_set_src(ui_navelectricclick, &ui_img_electric_on_png); break; // 4페이지: 대기전력
        case 4: lv_img_set_src(ui_navsettingclick, &ui_img_setting_on_png); break;   // 5페이지: 설정
    }
    // ==========================================
    // ⭐️ 2. [추가] 화살표 흐리게/진하게 자동 조절 로직
    // ==========================================
    int max_page_idx = 2; // 현재 page3(인덱스 2)이 끝이므로 2로 설정

    // [이전/위 화살표 처리]
    if (page_index == 0) {
        // 첫 페이지면 더 이상 위로 못 가니까 흐리게 (투명도 80 / 255)
        lv_obj_set_style_img_opa(ui_LeftArrow, 80, 0); 
    } else {
        // 아니면 선명하게 (투명도 255 / 255)
        lv_obj_set_style_img_opa(ui_LeftArrow, 255, 0);
    }

    // [다음/아래 화살표 처리]
    if (page_index == max_page_idx) {
        // 마지막 페이지면 더 이상 아래로 못 가니까 흐리게 (투명도 80 / 255)
        lv_obj_set_style_img_opa(ui_RightArrow, 80, 0);
    } else {
        // 아니면 선명하게 (투명도 255 / 255)
        lv_obj_set_style_img_opa(ui_RightArrow, 255, 0);
    }
}

// ==========================================
// ⭐️ 1. 공통 헬퍼: 현재 화면에 있는 페이지 번호(0, 1, 2)를 찾아주는 함수
// ==========================================
int get_current_page_index() {
    lv_coord_t scroll_y = lv_obj_get_scroll_y(ui_Mainscroll);
    lv_coord_t mid_point = scroll_y + (lv_obj_get_height(ui_Mainscroll) / 2);

    if (mid_point < lv_obj_get_y(ui_page2)) {
        return 0; // 1페이지 (조명)
    } else if (mid_point < lv_obj_get_y(ui_page3)) {
        return 1; // 2페이지 (밝기)
    } else {
        return 2; // 3페이지 (난방)
        // (나중에 page4, 5 만드시면 여기에 else if 만 추가하시면 됩니다!)
    }
}

// ==========================================
// ⭐️ 2. 기존 스크롤 센서 함수 (공통 헬퍼 덕분에 초간단해짐!)
// ==========================================
void mainscroll_event_cb(lv_event_t * e) {
    int current_idx = get_current_page_index(); // 지금 몇 페이지인지 알아옴
    update_nav_status(current_idx);             // 그에 맞춰 네비게이션 불 켜기!
}

// ==========================================
// ⭐️ 3. [위/왼쪽 화살표] 이전 페이지로 이동
// (기존 함수명 scroll_to_page1 유지)
// ==========================================
void scroll_to_page_prev(lv_event_t * e) {
    // 페이지들을 0, 1, 2번 방 배열에 묶어둡니다.
    lv_obj_t * pages[] = { ui_page1, ui_page2, ui_page3 };
    
    int current_idx = get_current_page_index();
    
    // 현재 1페이지(0번)가 아닐 때만 '이전'으로 한 칸 이동!
    if (current_idx > 0) {
        current_idx--;
        lv_obj_scroll_to_view(pages[current_idx], LV_ANIM_ON);
    }
}

// ==========================================
// ⭐️ 4. [아래/오른쪽 화살표] 다음 페이지로 이동
// (기존 함수명 scroll_to_page2 유지)
// ==========================================
void scroll_to_page_next(lv_event_t * e) {
    lv_obj_t * pages[] = { ui_page1, ui_page2, ui_page3 };
    int max_page_idx = 2; // 현재 page3(인덱스 2)이 끝이므로 최대 2
    
    int current_idx = get_current_page_index();
    
    // 현재 마지막 페이지(2번)가 아닐 때만 '다음'으로 한 칸 이동!
    if (current_idx < max_page_idx) {
        current_idx++;
        lv_obj_scroll_to_view(pages[current_idx], LV_ANIM_ON);
    }
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

        // ⭐️ [여기입니다!] 도화지를 그리자마자 수동으로 1페이지 불을 켭니다!
           update_nav_status(0);
        // ⭐️ [추가] ui_mainscroll 위젯에 "스크롤이 끝날 때(LV_EVENT_SCROLL_END)" 동작하는 센서 부착!
           lv_obj_add_event_cb(ui_Mainscroll, mainscroll_event_cb, LV_EVENT_SCROLL_END, NULL);
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


// ==========================================
// 2. 각 버튼을 눌렀을 때 실행되는 이벤트 함수들
// ==========================================
void nav_light_click(lv_event_t * e) {
    reset_nav_buttons(); // 일단 다 끄기
    lv_img_set_src(ui_navlightclick, &ui_img_light_on_png); // 얘만 파란색 켜기
    lv_obj_scroll_to_view(ui_page1, LV_ANIM_ON); // page1로 이동!
}

void nav_bright_click(lv_event_t * e) {
    reset_nav_buttons(); 
    lv_img_set_src(ui_navbrightclick, &ui_img_bright_on_png); 
    lv_obj_scroll_to_view(ui_page2, LV_ANIM_ON); // page2로 이동!
}

void nav_heat_click(lv_event_t * e) {
    reset_nav_buttons(); 
    lv_img_set_src(ui_navheatclick, &ui_img_heat_on_png); 
    lv_obj_scroll_to_view(ui_page3, LV_ANIM_ON); // 나중에 page3 만드시면 주석 해제!
}

void nav_electric_click(lv_event_t * e) {
    reset_nav_buttons(); 
    lv_img_set_src(ui_navelectricclick, &ui_img_electric_on_png); 
    // lv_obj_scroll_to_view(ui_page4, LV_ANIM_ON); 
}

void nav_setting_click(lv_event_t * e) {
    reset_nav_buttons(); 
    lv_img_set_src(ui_navsettingclick, &ui_img_setting_on_png); 
    // lv_obj_scroll_to_view(ui_page5, LV_ANIM_ON); 
}
