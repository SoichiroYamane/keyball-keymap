#include "ly_ctl_space.h"
#include "../user_keycode.h"

#define SWITCH_CTRL MOD_BIT(KC_LCTL)

enum {
  CTRL_DELAY = 20,
  SPACE_DELAY = 50,
  QUEUE_TIMEOUT = 250,
  QUEUE_LIMIT = 4,
};

static enum {
  SWITCH_IDLE,
  SWITCH_CTRL_DOWN,
  SWITCH_SPACE_DOWN,
  SWITCH_SPACE_UP,
} phase;
static uint16_t phase_timer;
static uint16_t queued_at;
static uint8_t queued_taps;

static void expire_queued_taps(void) {
  if (queued_taps && timer_elapsed(queued_at) >= QUEUE_TIMEOUT) {
    queued_taps = 0;
  }
}

static void advance_shortcut(void) {
  switch (phase) {
  case SWITCH_CTRL_DOWN:
    register_code(KC_SPC);
    phase = SWITCH_SPACE_DOWN;
    break;
  case SWITCH_SPACE_DOWN:
    unregister_code(KC_SPC);
    phase = SWITCH_SPACE_UP;
    break;
  case SWITCH_SPACE_UP:
    unregister_weak_mods(SWITCH_CTRL);
    phase = SWITCH_IDLE;
    break;
  default:
    return;
  }
  phase_timer = timer_read();
}

void finish_ly_ctl_space(void) {
  if (phase == SWITCH_IDLE) {
    return;
  }

  // QMK clears weak mods before dispatching a new press. Restore our Ctrl
  // for the remaining reports and finish in order without delaying matrix
  // scans. Normal, uninterrupted presses use the full timing below.
  add_weak_mods(SWITCH_CTRL);
  while (phase != SWITCH_IDLE) {
    advance_shortcut();
  }
}

void prepare_ly_ctl_space(uint16_t keycode, keyrecord_t *record) {
  finish_ly_ctl_space();
  if (record->event.pressed && keycode != LY_CTL_SPACE) {
    // Never apply a blocked switch after the user has started typing.
    queued_taps = 0;
  }
}

void process_ly_ctl_space(uint16_t keycode, keyrecord_t *record) {
  if (keycode == LY_CTL_SPACE && !record->event.pressed) {
    expire_queued_taps();
    if (!queued_taps) {
      queued_at = timer_read();
    }
    if (queued_taps < QUEUE_LIMIT) {
      ++queued_taps;
    }
    matrix_scan_ly_ctl_space();
  }
}

void matrix_scan_ly_ctl_space(void) {
  expire_queued_taps();
  if (phase == SWITCH_IDLE) {
    // Do not take over a held Space or turn Shift into Ctrl+Shift+Space.
    if (!queued_taps || has_anykey() || get_weak_mods() ||
        (get_mods() & ~MOD_MASK_CTRL)) {
      return;
    }
    --queued_taps;
    register_weak_mods(SWITCH_CTRL);
    phase = SWITCH_CTRL_DOWN;
    phase_timer = queued_at = timer_read();
  } else if (timer_elapsed(phase_timer) >=
             (phase == SWITCH_SPACE_DOWN ? SPACE_DELAY : CTRL_DELAY)) {
    advance_shortcut();
  }
}
