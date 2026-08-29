#pragma once

#include QMK_KEYBOARD_H

bool is_jp_toggle_held(void);
void process_jp_toggle(uint16_t keycode, keyrecord_t *record);
