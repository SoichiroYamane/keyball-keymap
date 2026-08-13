/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

Modified by Soichiro Yamane

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// #include "libs/combo/combo.h"
#include "libs/init/init.h"
#include "libs/layer/layer.h"
#include "libs/oled/oled.h"
#include "libs/user_keycode.h"

#include "quantum.h"

// Define the combo indices
enum combos {
  // AB_ESC,
  JK_TAB,
  // QW_SFT,
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

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal (
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , LT(7,KC_ENT),
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMMA , KC_DOT   , KC_MINUS ,
    KC_LCMD  , KC_LOPT  , ALT_TAB  ,LCTL_T(KC_TAB),LT(1,KC_SPC),LT(3,KC_BSPC),KC_LSFT,LY_TGML  , _______  , _______  , _______  , LY_CTL_SPACE
  ),

  [1] = LAYOUT_universal(
    S(KC_5)  , S(KC_7)  , S(KC_6)  , S(KC_4)  , KC_SLSH  ,                            KC_BSLS  ,S(KC_LBRC),S(KC_RBRC),S(KC_COMM), S(KC_DOT),
    S(KC_2)  , S(KC_3)  , S(KC_8)  , S(KC_EQL), KC_EQL   ,                           S(KC_MINS), S(KC_9)  , KC_S_0   , KC_SCLN  ,S(KC_SCLN),
    KC_GRV   , S(KC_GRV), S(KC_1)  ,S(KC_SLSH), XXXXXXX  ,                           S(KC_BSLS), KC_LBRC  , KC_RBRC  , KC_QUOT  ,S(KC_QUOT),
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , KC_BSPC  ,      XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , XXXXXXX
  ),

  [2] = LAYOUT_universal(
    XXXXXXX  , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                            KC_6     , KC_7     , KC_8     , KC_9     , KC_0     ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  ,C(KC_BSLS), KC_COMMA , KC_DOT   , XXXXXXX,
   C(KC_RBRC),C(KC_BSLS), XXXXXXX  ,S(KC_LCTL), ACMD_SP  , KC_BSPC  ,      XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , XXXXXXX
  ),

  [3] = LAYOUT_universal(
   S(C(KC_G)),TMUX_SHELL,TMUX_CPMOD,TMUX_OPWIN, XXXXXXX  ,                            KC_MCTL  ,A(S(KC_R)), A(KC_D)  , A(KC_T)  , KC_LPAD  ,
   S(KC_LALT), XXXXXXX  , XXXXXXX  , KC_LSFT  , TMUX_SPH ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , ENT_IMEVIM,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , TMUX_SPV ,                            KC_F20   , KC_F19   , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    DT_PRNT  , DT_DOWN  , DT_UP    , XXXXXXX  , XXXXXXX  , KC_BSPC  ,      KC_LSFT  , JP_TOGGLE, _______  , _______  , _______  , KC_MPLY
  ),

  // F19/F20 are semantic formatting events consumed by macOS Karabiner.
  // Keep physical N/M consistent between layer 3 and layer 5.

  // Mouse Layer
  [5] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            KC_MCTL  , XXXXXXX  , MS_BTN3  , XXXXXXX  , XXXXXXX,
    KC_LSFT  ,LCMD(KC_Z),LCMD(KC_C),LCMD(KC_V),LCMD(KC_F),                            LCMD(LSFT(KC_4)), MS_BTN1  , MS_BTN2  , MO(6)    , KC_ENT ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            KC_F20   , KC_F19   , MS_BTN5  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , TG(5)    , XXXXXXX  , XXXXXXX  , XXXXXXX  , KC_CAPS
  ),

  // dummy layer for scrolling
  [6] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            SSNP_FRE , SSNP_VRT , SSNP_HOR , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),

  // OmniWM workspace, focus, and window-management shortcuts.
  // The chords match modules/home/darwin/omniwm/keymap.nix in the nix repo.
  [7] = LAYOUT_universal(
    RGUI(KC_1), RGUI(KC_2), RGUI(KC_3), RGUI(KC_4), RGUI(KC_5),                  RGUI(KC_6), RGUI(KC_7), RGUI(KC_8), RGUI(KC_9), XXXXXXX,
    XXXXXXX  , LCMD(KC_Z), LCMD(KC_C), LCMD(KC_V), LCMD(KC_F),                  RAG(KC_H),  RAG(KC_J),  RAG(KC_K),  RAG(KC_L),  XXXXXXX,
    RSG(KC_1), RSG(KC_2), RSG(KC_3), RSG(KC_4), RSG(KC_5),                      RSG(KC_6), RSG(KC_7), RSG(KC_8), RSG(KC_9), XXXXXXX,
    RGUI(KC_F), XXXXXXX  , XXXXXXX  , MO(8)    , MO(9)    , MO(10)    ,        XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),

  // Temporary sublayers for the three layer-7 mode keys.  Transparent keys
  // keep layer-7 workspace/focus shortcuts available while a mode is held.
  [8] = LAYOUT_universal(
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,                              RGUI(KC_H), _______, _______, RGUI(KC_L), _______,
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______
  ),

  [9] = LAYOUT_universal(
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,                              RSG(KC_H),  _______, _______, RSG(KC_L),  _______,
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______
  ),

  [10] = LAYOUT_universal(
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,                              RCG(KC_H),  _______, _______, RCG(KC_L),  _______,
    _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______
  ),
};
