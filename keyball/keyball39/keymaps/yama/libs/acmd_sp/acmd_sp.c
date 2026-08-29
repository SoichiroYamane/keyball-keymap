#include "acmd_sp.h"
#include "../user_keycode.h"

static uint32_t acmd_sp_timer = 0;
static uint16_t acmd_sp_tapping_term = TAPPING_TERM;
static bool is_acmd_sp_trigger = false;
static bool is_acmd_sp_hold_active = false;

static const uint8_t ACMD_SP_MODS = MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI);

static void settle_acmd_sp_hold(void) {
  if (!is_acmd_sp_hold_active) {
    register_weak_mods(ACMD_SP_MODS);
    is_acmd_sp_hold_active = true;
  }
  is_acmd_sp_trigger = false;
}

void process_acmd_sp(uint16_t keycode, keyrecord_t *record) {
  if (keycode == ACMD_SP) {
    if (record->event.pressed) {
      acmd_sp_timer = timer_read();
      acmd_sp_tapping_term = GET_TAPPING_TERM(keycode, record);
      is_acmd_sp_trigger = true;
      is_acmd_sp_hold_active = false;
    } else {
      if (is_acmd_sp_hold_active) {
        unregister_weak_mods(ACMD_SP_MODS);
        is_acmd_sp_hold_active = false;
      } else {
        tap_code(KC_SPC);
      }
      is_acmd_sp_trigger = false;
    }
  } else if (record->event.pressed && is_acmd_sp_trigger) {
    // Match HOLD_ON_OTHER_KEY_PRESS for this custom tap/hold key.
    settle_acmd_sp_hold();
  }
}

void matrix_scan_acmd_sp(void) {
  if (is_acmd_sp_trigger) {
    if (timer_elapsed(acmd_sp_timer) >= acmd_sp_tapping_term) {
      settle_acmd_sp_hold();
    }
  }
}
