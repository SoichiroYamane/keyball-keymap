#pragma once

#include QMK_KEYBOARD_H

void process_shift_tgml(uint16_t keycode, keyrecord_t *record);
void matrix_scan_user(void);
