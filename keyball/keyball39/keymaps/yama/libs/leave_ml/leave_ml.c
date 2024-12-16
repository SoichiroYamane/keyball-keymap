#include "leave_ml.h"
#include "../user_keycode.h"

void process_leave_ml(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LEAVE_ML) {
    if (record->event.pressed) {
      layer_on(0);
      // Disable the KC_ESC registration
      // Very annoying when filling in forms
      // register_code(KC_ESC);
      register_code(KC_LNG2);
    }
  }
}
