#include "jp_toggle.h"
#include "../user_keycode.h"

static inline void toggle_language(bool activate_lang1) {
  if (activate_lang1) {
    tap_code(KC_LNG1);
  } else {
    tap_code(KC_LNG2);
  }
}

// This is physical-key intent only. Firmware cannot observe the host's
// current input-source state.
static bool jp_toggle_held = false;

bool is_jp_toggle_held(void) { return jp_toggle_held; }

void process_jp_toggle(uint16_t keycode, keyrecord_t *record) {
  if (keycode == JP_TOGGLE) {
    if (record->event.pressed) {
      toggle_language(true);
      jp_toggle_held = true;
    } else {
      toggle_language(false);
      jp_toggle_held = false;
    }
  }
}
