#include "others.h"
#include "../jp_toggle/jp_toggle.h"
#include "../user_keycode.h"

static inline void tap_code_combination(uint16_t mod, uint16_t key,
                                        bool condition) {
  if (condition) {
    tap_code(mod);
  } else {
    tap_code16(key);
  }
}

void process_ent_imevim(uint16_t keycode, keyrecord_t *record) {
  if (keycode == ENT_IMEVIM) {
    if (record->event.pressed) {
      tap_code_combination(KC_ENT, C(KC_Y), is_jp_toggle_held());
    }
  }
}

void process_alt_tab(uint16_t keycode, keyrecord_t *record) {
  if (keycode == ALT_TAB) {
    if (record->event.pressed) {
      // Use weak Alt so an independently held Alt remains held. Send the
      // reports immediately; this shortcut must not block matrix scanning.
      register_code16(A(KC_TAB));
      unregister_code16(A(KC_TAB));
    }
  }
}

void process_kc_s_0(uint16_t keycode, keyrecord_t *record) {
  if (keycode == KC_S_0) {
    if (record->event.pressed) {
      tap_code16(S(KC_0));
    }
  }
}
