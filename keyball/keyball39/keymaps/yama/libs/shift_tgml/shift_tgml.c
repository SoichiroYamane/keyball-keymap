#include "shift_tgml.h"
#include "../user_keycode.h"

static uint32_t s_tgml_timer = 0;
static bool is_s_tgml_trigger = false;
static bool is_s_tgml_hold_active = false;

void process_shift_tgml(uint16_t keycode, keyrecord_t *record) {
  if (keycode == S_TGML) {
    if (record->event.pressed) {
      s_tgml_timer = timer_read();
      is_s_tgml_trigger = true;
    } else {
      if (is_s_tgml_hold_active) {
        unregister_code(KC_LSFT);
        is_s_tgml_hold_active = false;
      } else {
        layer_on(5);
      }
      is_s_tgml_trigger = false;
    }
  }
}

void matrix_scan_user(void) {
  if (is_s_tgml_trigger) {
    if (timer_elapsed(s_tgml_timer) > TAPPING_TERM) {
      register_code(KC_LSFT);
      is_s_tgml_hold_active = true;
      is_s_tgml_trigger = false;
    }
  }
}
