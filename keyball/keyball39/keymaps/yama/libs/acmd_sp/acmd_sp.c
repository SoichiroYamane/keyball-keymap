#include "acmd_sp.h"
#include "../user_keycode.h"

static uint32_t acmd_sp_timer = 0;
static bool is_acmd_sp_trigger = false;
static bool is_acmd_sp_hold_active = false;

void process_acmd_sp(uint16_t keycode, keyrecord_t *record) {
  if (keycode == ACMD_SP) {
    if (record->event.pressed) {
      acmd_sp_timer = timer_read();
      is_acmd_sp_trigger = true;
    } else {
      if (is_acmd_sp_hold_active) {
        unregister_code16(A(KC_LCMD));
        is_acmd_sp_hold_active = false;
      } else {
        tap_code(KC_SPC);
      }
      is_acmd_sp_trigger = false;
    }
  }
}

void matrix_scan_acmd_sp(void) {
  if (is_acmd_sp_trigger) {
    if (timer_elapsed(acmd_sp_timer) > TAPPING_TERM) {
      register_code16(A(KC_LCMD));
      is_acmd_sp_hold_active = true;
      is_acmd_sp_trigger = false;
    }
  }
}
