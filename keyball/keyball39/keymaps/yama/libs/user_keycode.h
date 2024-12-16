#pragma once
#include QMK_KEYBOARD_H

// JP_TOGGLE などのカスタムキーコードを定義
enum custom_keycodes {
  JP_TOGGLE = SAFE_RANGE,
  TMUX_OPWIN,
  TMUX_SPH,
  TMUX_SPV,
  TMUX_CPMOD,
  TMUX_SHELL,
  ENT_IMEVIM,
  ALT_TAB,
  // LEAVE_ML,
  HM_TOG,
  KC_S_0,
  // S_TGML,
  LY_TGML,
  ACMD_SP,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record);
void matrix_scan_user(void);
