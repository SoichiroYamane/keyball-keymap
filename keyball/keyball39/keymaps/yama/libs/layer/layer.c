#include "layer.h"

layer_state_t layer_state_set_user(layer_state_t state) {
  // Auto enable scroll mode when the highest layer is 3
  keyball_set_scroll_mode(get_highest_layer(state) == 6);

  uint8_t layer = biton32(state);
  switch (layer) {
  // case 1:
  //   rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  //   rgb_matrix_sethsv_noeeprom(0, 255, 128); // RED
  //   break;
  //
  // case 2:
  //   rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  //   rgb_matrix_sethsv_noeeprom(170, 255, 128); // BLUE
  //   break;
  //
  // case 3:
  //   rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  //   rgb_matrix_sethsv_noeeprom(85, 255, 128); // GREEN
  //   break;
  //
  case 4:
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(191, 255, 128); // PURPLE
    break;

  // mouse layer
  case 5:
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(0, 255, 128); // RED
    break;

  // dummy layer for scrolling
  case 6:
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(128, 255, 128); // CYAN
    break;

  // cmd layer
  case 7:
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(85, 255, 128); // GREEN
    break;

  default:
    rgb_matrix_reload_from_eeprom();
    break;
  }

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
  keyball_handle_auto_mouse_layer_change(state);
#endif

  return state;
}
