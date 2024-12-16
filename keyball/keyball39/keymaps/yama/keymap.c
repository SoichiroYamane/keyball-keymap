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
    KC_LCMD  , KC_LOPT  , ALT_TAB  ,LCTL_T(KC_TAB),LT(1,KC_SPC),LT(3,KC_BSPC),KC_LSFT,LY_TGML  , _______  , _______  , _______  ,LT(4, KC_CAPS)
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
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , TMUX_SPV ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    DT_PRNT  , DT_DOWN  , DT_UP    , XXXXXXX  , XXXXXXX  , KC_BSPC  ,      XXXXXXX  , JP_TOGGLE, _______  , _______  , _______  , KC_MPLY
  ),

  [4] = LAYOUT_universal(
    AML_TO   , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  ,LCMD(C(KC_S)),XXXXXXX,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    XXXXXXX  ,LCMD(KC_MINS),LCMD(KC_EQL),KC_VOLD,KC_VOLU ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,
    HM_TOG   , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , XXXXXXX  , _______  , _______  , _______  , XXXXXXX
  ),


  // Mouse Layer
  [5] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            KC_MCTL  , XXXXXXX  , KC_BTN3  , XXXXXXX  , XXXXXXX,
    KC_LSFT  ,LCMD(KC_Z),LCMD(KC_C),LCMD(KC_V),LCMD(KC_F),                            XXXXXXX  , KC_BTN1  , KC_BTN2  , MO(6)    , KC_ENT ,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , KC_BTN4  , KC_BTN5  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , TG(5)    , XXXXXXX  , XXXXXXX  , XXXXXXX  , KC_CAPS
  ),

  // dummy layer for scrolling
  [6] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),

  [7] = LAYOUT_universal(
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  ,LCMD(KC_Z),LCMD(KC_C),LCMD(KC_V),LCMD(KC_F),                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,                            XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX,
    XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  ,      XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX  , XXXXXXX
  ),
};

