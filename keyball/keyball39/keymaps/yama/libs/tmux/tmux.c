#include "tmux.h"
#include "../user_keycode.h"

static inline void tap_tmux_sequence(uint16_t mod, uint16_t key) {
  tap_code16(mod);
  tap_code16(key);
}

bool process_tmux_keycodes(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {
    return true;
  }

  switch (keycode) {
  case TMUX_OPWIN:
    tap_tmux_sequence(C(KC_BSLS), KC_C);
    break;
  case TMUX_SPH:
    tap_tmux_sequence(C(KC_BSLS), S(KC_5));
    break;
  case TMUX_SPV:
    tap_tmux_sequence(C(KC_BSLS), S(KC_QUOT));
    break;
  case TMUX_CPMOD:
    tap_tmux_sequence(C(KC_BSLS), KC_LBRC);
    break;
  case TMUX_SHELL:
    tap_tmux_sequence(C(KC_BSLS), S(KC_SCLN));
    break;
  default:
    return true;
  }
  return false;
}
