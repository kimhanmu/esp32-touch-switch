SET(SOURCES components/ui_comp_imgbutton1.c
    components/ui_comp.c
    screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_circuit1_off_png.c
    images/ui_img_circuit1_on_png.c
    images/ui_img_1732143338.c
    images/ui_img_345358842.c
    images/ui_img_1723991977.c
    images/ui_img_345601881.c
    images/ui_img_689030760.c
    images/ui_img_1144295435.c)

add_library(ui ${SOURCES})
