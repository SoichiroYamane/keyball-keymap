#pragma once

#include QMK_KEYBOARD_H

void finish_ly_ctl_space(void);
void prepare_ly_ctl_space(uint16_t keycode, keyrecord_t *record);
void process_ly_ctl_space(uint16_t keycode, keyrecord_t *record);
void matrix_scan_ly_ctl_space(void);
