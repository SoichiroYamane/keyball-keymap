// libs/combo/combo.c
#include "combo.h"

// Define the combo indices
enum combos {
  AB_ESC,
  JK_TAB,
  QW_SFT,
  // SD_LAYER,
};

// Define the combo sequences
// const uint16_t PROGMEM ab_combo[] = {KC_A, KC_B, COMBO_END};
const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
// const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};
// const uint16_t PROGMEM sd_combo[] = {KC_S, KC_D, COMBO_END};

// Define the combos array
combo_t key_combos[] = {
    // [AB_ESC] = COMBO(ab_combo, KC_ESC),
    [JK_TAB] = COMBO(jk_combo, KC_ESC),
    // [QW_SFT] = COMBO(qw_combo, KC_LSFT),
    // [SD_LAYER] = COMBO(sd_combo, MO(_LAYER)),
};
