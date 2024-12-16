#pragma once

#include QMK_KEYBOARD_H

void process_acmd_sp(uint16_t keycode, keyrecord_t *record);
void matrix_scan_acmd_sp(void);
