#include "ly_ctl_space.h"
#include "../user_keycode.h"

void process_ly_ctl_space(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LY_CTL_SPACE) {
    if (!record->event.pressed) {
      // Keep the key registered long enough for the host input source
      // shortcut to observe both the modifier and Space reports.
      tap_code16_delay(C(KC_SPC), 50);
    }
  }
}
