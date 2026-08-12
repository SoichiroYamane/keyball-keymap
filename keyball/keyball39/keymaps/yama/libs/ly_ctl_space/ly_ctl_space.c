#include "ly_ctl_space.h"
#include "../user_keycode.h"

static uint32_t ly_ctl_space_timer = 0;
static bool is_ly_ctl_space_trigger = false;
static bool is_ly_ctl_space_hold_active = false;

void process_ly_ctl_space(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LY_CTL_SPACE) {
    if (record->event.pressed) {
      ly_ctl_space_timer = timer_read();
      is_ly_ctl_space_trigger = true;
    } else {
      // Stop the hold check before emitting the tap. `tap_code16_delay` uses
      // weak modifiers, so an already-held physical Ctrl is not cleared by
      // this synthetic Ctrl+Space event.
      is_ly_ctl_space_trigger = false;
      if (is_ly_ctl_space_hold_active) {
        layer_off(4);
        is_ly_ctl_space_hold_active = false;
      } else {
        // Keep the key registered long enough for the host input source
        // shortcut to observe both the modifier and Space reports.
        tap_code16_delay(C(KC_SPC), 50);
      }
    }
  }
}

void matrix_scan_ly_ctl_space(void) {
  if (is_ly_ctl_space_trigger) {
    if (timer_elapsed(ly_ctl_space_timer) > TAPPING_TERM) {
      layer_on(4);
      is_ly_ctl_space_hold_active = true;
      is_ly_ctl_space_trigger = false;
    }
  }
}
