#include "init.h"

void keyboard_post_init_user(void) {
  // Call the post init code.
  rgb_matrix_reload_from_eeprom();

  // Enable auto mouse layer
  // set_auto_mouse_enable(true);
}
