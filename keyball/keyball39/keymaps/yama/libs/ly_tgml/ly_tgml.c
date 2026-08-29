#include "ly_tgml.h"
#include "../user_keycode.h"

static uint32_t ly_tgml_timer = 0;
static uint16_t ly_tgml_tapping_term = TAPPING_TERM;
static bool is_ly_tgml_trigger = false;
static bool is_ly_tgml_hold_active = false;
static bool is_ly_tgml_layer2_owned = false;

static void settle_ly_tgml_hold(void) {
  is_ly_tgml_layer2_owned = !layer_state_is(2);
  if (is_ly_tgml_layer2_owned) {
    layer_on(2);
  }
  is_ly_tgml_hold_active = true;
  is_ly_tgml_trigger = false;
}

void process_ly_tgml(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LY_TGML) {
    if (record->event.pressed) {
      ly_tgml_timer = timer_read();
      ly_tgml_tapping_term = GET_TAPPING_TERM(keycode, record);
      is_ly_tgml_trigger = true;
      is_ly_tgml_hold_active = false;
    } else {
      if (is_ly_tgml_hold_active) {
        if (is_ly_tgml_layer2_owned) {
          layer_off(2);
        }
        is_ly_tgml_hold_active = false;
        is_ly_tgml_layer2_owned = false;
      } else {
        // LY_TGML intentionally serves as the mouse-operation layer toggle:
        // a tap enables mouse layer 5, while its hold behavior uses layer 2.
        layer_on(5);
      }
      is_ly_tgml_trigger = false;
    }
  } else if (record->event.pressed && is_ly_tgml_trigger) {
    // A roll means the key is being used as a mouse-operation hold, not a
    // tap that should leave mouse layer 5 enabled.
    settle_ly_tgml_hold();
  }
}

void matrix_scan_ly_tgml(void) {
  if (is_ly_tgml_trigger) {
    if (timer_elapsed(ly_tgml_timer) >= ly_tgml_tapping_term) {
      settle_ly_tgml_hold();
    }
  }
}
