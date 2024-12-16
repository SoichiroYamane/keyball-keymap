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
      tap_code_combination(KC_ENT, C(KC_Y), is_lang1_active());
    }
  }
}

void process_alt_tab(uint16_t keycode, keyrecord_t *record) {
  if (keycode == ALT_TAB) {
    if (record->event.pressed) {
      register_code(KC_LALT);
      tap_code16_delay(KC_TAB, 50);
      unregister_code(KC_LALT);
    }
  }
}

void process_kc_s_0(uint16_t keycode, keyrecord_t *record) {
  if (keycode == KC_S_0) {
    if (record->event.pressed) {
      register_code(KC_LSFT);
      tap_code(KC_0);
      unregister_code(KC_LSFT);
    }
  }
}
