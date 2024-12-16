#pragma once

#include QMK_KEYBOARD_H

bool is_lang1_active(void);
void process_jp_toggle(uint16_t keycode, keyrecord_t *record);
