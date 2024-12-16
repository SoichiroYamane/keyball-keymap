#include "jp_toggle.h"
#include "../user_keycode.h"

static inline void toggle_language(bool activate_lang1) {
  if (activate_lang1) {
    tap_code(KC_LNG1);
  } else {
    tap_code(KC_LNG2);
  }
}

static bool lang1_active = false;

bool inline is_lang1_active(void) { return lang1_active; }

void process_jp_toggle(uint16_t keycode, keyrecord_t *record) {
  if (keycode == JP_TOGGLE) {
    if (record->event.pressed) {
      toggle_language(true);
      lang1_active = true;
    } else {
      toggle_language(false);
      lang1_active = false;
    }
  }
}
