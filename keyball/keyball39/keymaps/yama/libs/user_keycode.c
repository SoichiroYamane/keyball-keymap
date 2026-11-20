#include "acmd_sp/acmd_sp.h"
#include "hm_tog/hm_tog.h"
#include "jp_toggle/jp_toggle.h"
// #include "leave_ml/leave_ml.h"
#include "others/others.h"
// #include "shift_tgml/shift_tgml.h"
#include "ly_ctl_space/ly_ctl_space.h"
#include "ly_tgml/ly_tgml.h"
#include "tmux/tmux.h"

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
  prepare_ly_ctl_space(keycode, record);
  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Buffered events can emit reports here. Only the pre-hook sees physical
  // presses in order, so a replay must not cancel a newer queued switch.
  finish_ly_ctl_space();
  process_acmd_sp(keycode, record);
  process_jp_toggle(keycode, record);
  process_ly_tgml(keycode, record);
  process_ly_ctl_space(keycode, record);
  process_hm_tog(keycode, record);
  process_ent_imevim(keycode, record);
  process_alt_tab(keycode, record);
  process_kc_s_0(keycode, record);

  // for optimization, check if statement first
  if (!process_tmux_keycodes(keycode, record)) {
    return false;
  }
  return true;
}

void matrix_scan_user(void) {
  matrix_scan_acmd_sp();
  matrix_scan_ly_tgml();
  matrix_scan_ly_ctl_space();
}
