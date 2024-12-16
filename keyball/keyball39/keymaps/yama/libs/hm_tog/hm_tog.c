#include "hm_tog.h"
#include "../user_keycode.h"

void process_hm_tog(uint16_t keycode, keyrecord_t *record) {
  if (keycode == HM_TOG) {
    if (record->event.pressed) {
      if (rgb_matrix_is_enabled()) {
        // if RGB is enabled, disable it
        rgb_matrix_disable();
      } else {
        // if RGB is disabled, enable it
        // and set the RGB mode to HEATMAP
        rgb_matrix_enable();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
      }
    }
  }
}
