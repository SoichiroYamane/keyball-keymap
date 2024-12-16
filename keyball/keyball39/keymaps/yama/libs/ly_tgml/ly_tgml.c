#include "ly_tgml.h"
#include "../user_keycode.h"

static uint32_t ly_tgml_timer = 0;
static bool is_ly_tgml_trigger = false;
static bool is_ly_tgml_hold_active = false;

void process_ly_tgml(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LY_TGML) {
    if (record->event.pressed) {
      ly_tgml_timer = timer_read();
      is_ly_tgml_trigger = true;
    } else {
      if (is_ly_tgml_hold_active) {
        // unregister_code(KC_LSFT);
        layer_off(2);
        is_ly_tgml_hold_active = false;
      } else {
        layer_on(5);
      }
      is_ly_tgml_trigger = false;
    }
  }
}

void matrix_scan_ly_tgml(void) {
  if (is_ly_tgml_trigger) {
    if (timer_elapsed(ly_tgml_timer) > TAPPING_TERM) {
      layer_on(2);
      is_ly_tgml_hold_active = true;
      is_ly_tgml_trigger = false;
    }
  }
}
