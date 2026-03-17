# main 폴더와 ui 폴더(하위 폴더 포함) 내의 모든 .c 파일을 자동으로 찾습니다.
file(GLOB_RECURSE srcs "*.c" "ui/*.c")

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS "." "ui"
    REQUIRES esp_lcd driver esp_timer
)