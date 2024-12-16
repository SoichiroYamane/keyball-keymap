#pragma once

#include QMK_KEYBOARD_H

void process_ly_tgml(uint16_t keycode, keyrecord_t *record);
void matrix_scan_ly_tgml(void);
