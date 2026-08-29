#pragma once

#include <stdbool.h>
#include <stdint.h>

#define SAFE_RANGE 0x1000u
#define TAPPING_TERM 100u
#define QK_MODS 0x8000u

#define KC_NO 0x00u
#define KC_A 0x04u
#define KC_0 0x27u
#define KC_Y 0x1cu
#define KC_TAB 0x2bu
#define KC_ENT 0x28u
#define KC_SPC 0x2cu
#define KC_LSFT 0xe1u
#define KC_LALT 0xe2u
#define KC_LCMD 0xe3u
#define KC_LGUI KC_LCMD
#define KC_LNG1 0x90u
#define KC_LNG2 0x91u

#define C(keycode) (0x1000u | (keycode))
#define A(keycode) (0x2000u | (keycode))
#define S(keycode) (0x3000u | (keycode))

#define MOD_BIT(keycode) \
    ((keycode) == KC_LALT ? 0x04u : ((keycode) == KC_LGUI ? 0x08u : 0u))

#define GET_TAPPING_TERM(keycode, record) (g_tapping_term)

typedef struct {
    struct {
        bool pressed;
        uint16_t time;
    } event;
} keyrecord_t;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t h;
    int8_t v;
    uint8_t buttons;
} report_mouse_t;

extern uint16_t g_now;
extern uint16_t g_tapping_term;

uint16_t timer_read(void);
uint16_t timer_elapsed(uint16_t timer);

void register_code16(uint16_t code);
void unregister_code16(uint16_t code);
void register_code(uint8_t code);
void unregister_code(uint8_t code);
void tap_code(uint8_t code);
void tap_code16(uint16_t code);
void tap_code16_delay(uint16_t code, uint16_t delay);
void register_weak_mods(uint8_t mods);
void unregister_weak_mods(uint8_t mods);
void layer_on(uint8_t layer);
void layer_off(uint8_t layer);
bool layer_state_is(uint8_t layer);
