#include "ly_ctl_space.h"
#include "../user_keycode.h"

enum {
  LY_CTL_SPACE_DELAY = 50,
  LY_CTL_SPACE_MODIFIER_DELAY = 20,
};

static uint16_t ly_ctl_space_timer;
static uint16_t ly_ctl_space_modifier_timer;
static bool ly_ctl_space_pending;
static bool ly_ctl_space_modifier_pending;

void process_ly_ctl_space(uint16_t keycode, keyrecord_t *record) {
  if (keycode != LY_CTL_SPACE || record->event.pressed) {
    return;
  }

  // Keep the host-visible Ctrl+Space reports alive without blocking the
  // keyboard matrix scan. LY_CTL_SPACE is a language-switch key, not a
  // mouse-operation key; the release windows are serviced from matrix_scan.
  if (ly_ctl_space_pending) {
    unregister_code(KC_SPC);
    ly_ctl_space_pending = false;
  }
  if (ly_ctl_space_modifier_pending) {
    // A repeated tap reuses the weak modifier for the new chord. Keep it
    // registered until that chord's own delayed release is scheduled.
    ly_ctl_space_modifier_pending = false;
  }
  register_code16(C(KC_SPC));
  ly_ctl_space_timer = timer_read();
  ly_ctl_space_pending = true;
}

void matrix_scan_ly_ctl_space(void) {
  if (ly_ctl_space_pending &&
      timer_elapsed(ly_ctl_space_timer) >= LY_CTL_SPACE_DELAY) {
    unregister_code(KC_SPC);
    ly_ctl_space_pending = false;
    ly_ctl_space_modifier_timer = timer_read();
    ly_ctl_space_modifier_pending = true;
  }
  if (ly_ctl_space_modifier_pending &&
      timer_elapsed(ly_ctl_space_modifier_timer) >=
          LY_CTL_SPACE_MODIFIER_DELAY) {
    unregister_weak_mods(MOD_BIT(KC_LCTL));
    ly_ctl_space_modifier_pending = false;
  }
}
