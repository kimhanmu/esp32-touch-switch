
# 1. main 폴더와 그 하위 폴더(ui 포함)에 있는 "모든 .c 파일"을 싹 다 찾습니다.
file(GLOB_RECURSE all_sources "*.c")

# 2. 좀비 파일 ui_events.c 만 명단에서 강제로 제외합니다.
list(FILTER all_sources EXCLUDE REGEX "ui_events\\.c$")

# 3. 누락되는 파일 없이 긁어온 모든 파일로 빌드를 진행합니다.
idf_component_register(SRCS ${all_sources}
                       INCLUDE_DIRS "." "ui"
                       REQUIRES esp_lcd driver esp_timer)