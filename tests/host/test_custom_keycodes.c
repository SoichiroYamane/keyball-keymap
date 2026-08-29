#include <assert.h>
#include <stdio.h>
#include <string.h>

#define QMK_KEYBOARD_H "qmk_test_stub.h"
#include "qmk_test_stub.h"

uint16_t g_now = 0;
uint16_t g_tapping_term = TAPPING_TERM;

struct call {
    const char *name;
    uint16_t code;
    uint16_t value;
};

static struct call calls[32];
static size_t call_count;
static uint8_t active_layers;

static void reset_fixture(void) {
    memset(calls, 0, sizeof(calls));
    call_count = 0;
    active_layers = 0;
    g_now = 0;
    g_tapping_term = TAPPING_TERM;
}

static void record_call(const char *name, uint16_t code, uint16_t value) {
    assert(call_count < sizeof(calls) / sizeof(calls[0]));
    calls[call_count++] = (struct call){name, code, value};
}

static bool saw_call(const char *name, uint16_t code) {
    for (size_t i = 0; i < call_count; ++i) {
        if (strcmp(calls[i].name, name) == 0 && calls[i].code == code) {
            return true;
        }
    }
    return false;
}

static bool saw_name(const char *name) {
    for (size_t i = 0; i < call_count; ++i) {
        if (strcmp(calls[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

static keyrecord_t record(bool pressed) {
    return (keyrecord_t){.event = {.pressed = pressed, .time = g_now}};
}

uint16_t timer_read(void) { return g_now; }
uint16_t timer_elapsed(uint16_t timer) { return (uint16_t)(g_now - timer); }

void register_code16(uint16_t code) { record_call("register_code16", code, 0); }
void unregister_code16(uint16_t code) { record_call("unregister_code16", code, 0); }
void register_code(uint8_t code) { record_call("register_code", code, 0); }
void unregister_code(uint8_t code) { record_call("unregister_code", code, 0); }
void tap_code(uint8_t code) { record_call("tap_code", code, 0); }
void tap_code16(uint16_t code) { record_call("tap_code16", code, 0); }
void tap_code16_delay(uint16_t code, uint16_t delay) {
    record_call("tap_code16_delay", code, delay);
}
void register_weak_mods(uint8_t mods) { record_call("register_weak_mods", mods, 0); }
void unregister_weak_mods(uint8_t mods) { record_call("unregister_weak_mods", mods, 0); }
void layer_on(uint8_t layer) { active_layers |= (uint8_t)(1u << layer); }
void layer_off(uint8_t layer) { active_layers &= (uint8_t)~(1u << layer); }
bool layer_state_is(uint8_t layer) {
    return (active_layers & (uint8_t)(1u << layer)) != 0;
}

#include "../../keyball/keyball39/keymaps/yama/libs/ly_ctl_space/ly_ctl_space.c"
#include "../../keyball/keyball39/keymaps/yama/libs/acmd_sp/acmd_sp.c"
#include "../../keyball/keyball39/keymaps/yama/libs/ly_tgml/ly_tgml.c"
#include "../../keyball/keyball39/keymaps/yama/libs/jp_toggle/jp_toggle.c"
#include "../../keyball/keyball39/keymaps/yama/libs/others/others.c"

void matrix_scan_user(void) {
    matrix_scan_acmd_sp();
    matrix_scan_ly_tgml();
    matrix_scan_ly_ctl_space();
}

static void test_ctl_space_uses_separated_release(void) {
    reset_fixture();
    keyrecord_t release = record(false);
    process_ly_ctl_space(LY_CTL_SPACE, &release);

    assert(call_count == 1);
    assert(saw_call("register_code16", C(KC_SPC)));
    assert(!saw_name("tap_code16_delay"));
    assert(!saw_call("unregister_code16", C(KC_SPC)));

    g_now = 49;
    matrix_scan_user();
    assert(call_count == 1);
    assert(!saw_call("unregister_code", KC_SPC));
    assert(!saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));

    g_now = 50;
    matrix_scan_user();
    assert(call_count == 2);
    assert(saw_call("unregister_code", KC_SPC));
    assert(!saw_call("unregister_code16", C(KC_SPC)));
    assert(!saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));

    g_now = 69;
    matrix_scan_user();
    assert(call_count == 2);
    assert(!saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));

    g_now = 70;
    matrix_scan_user();
    assert(call_count == 3);
    assert(saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));
}

static void test_ctl_space_rapid_repeat_keeps_modifier(void) {
    reset_fixture();
    keyrecord_t release = record(false);
    process_ly_ctl_space(LY_CTL_SPACE, &release);

    g_now = 50;
    matrix_scan_user();
    assert(call_count == 2);

    g_now = 55;
    release = record(false);
    process_ly_ctl_space(LY_CTL_SPACE, &release);
    assert(call_count == 3);

    g_now = 69;
    matrix_scan_user();
    assert(call_count == 3);
    assert(!saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));

    g_now = 104;
    matrix_scan_user();
    assert(call_count == 3);

    g_now = 105;
    matrix_scan_user();
    assert(call_count == 4);
    assert(saw_call("unregister_code", KC_SPC));
    assert(!saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));

    g_now = 124;
    matrix_scan_user();
    assert(call_count == 4);

    g_now = 125;
    matrix_scan_user();
    assert(call_count == 5);
    assert(saw_call("unregister_weak_mods", MOD_BIT(KC_LCTL)));
}

static void test_acmd_sp_uses_weak_mods_and_roll_hold(void) {
    reset_fixture();
    keyrecord_t press = record(true);
    process_acmd_sp(ACMD_SP, &press);

    g_now = 10;
    process_acmd_sp(KC_A, &press);
    assert(saw_call("register_weak_mods", MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)));
    assert(!saw_call("register_code16", A(KC_LCMD)));

    keyrecord_t release = record(false);
    process_acmd_sp(ACMD_SP, &release);
    assert(saw_call("unregister_weak_mods", MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)));
}

static void test_custom_tap_hold_uses_dynamic_term(void) {
    reset_fixture();
    g_tapping_term = 200;
    keyrecord_t press = record(true);
    process_acmd_sp(ACMD_SP, &press);

    g_now = 150;
    matrix_scan_user();
    assert(!saw_call("register_weak_mods", MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)));

    g_now = 200;
    matrix_scan_user();
    assert(saw_call("register_weak_mods", MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI)));

    keyrecord_t release = record(false);
    process_acmd_sp(ACMD_SP, &release);
}

static void test_ly_tgml_roll_is_mouse_hold(void) {
    reset_fixture();
    keyrecord_t press = record(true);
    process_ly_tgml(LY_TGML, &press);

    g_now = 10;
    process_ly_tgml(KC_A, &press);
    assert((active_layers & (1u << 2)) != 0);
    assert((active_layers & (1u << 5)) == 0);

    keyrecord_t release = record(false);
    process_ly_tgml(LY_TGML, &release);
    assert((active_layers & (1u << 2)) == 0);
}

static void test_ly_tgml_tap_still_enables_mouse_layer(void) {
    reset_fixture();
    keyrecord_t press = record(true);
    process_ly_tgml(LY_TGML, &press);

    g_now = 50;
    keyrecord_t release = record(false);
    process_ly_tgml(LY_TGML, &release);
    assert((active_layers & (1u << 5)) != 0);
}

static void test_ly_tgml_does_not_release_external_layer(void) {
    reset_fixture();
    layer_on(2);
    keyrecord_t press = record(true);
    process_ly_tgml(LY_TGML, &press);

    g_now = 10;
    process_ly_tgml(KC_A, &press);
    keyrecord_t release = record(false);
    process_ly_tgml(LY_TGML, &release);
    assert((active_layers & (1u << 2)) != 0);
}

static void test_alt_tab_and_shift_zero_preserve_modifiers(void) {
    reset_fixture();
    keyrecord_t press = record(true);

    process_alt_tab(ALT_TAB, &press);
    assert(saw_call("register_code16", A(KC_TAB)));
    assert(saw_call("unregister_code16", A(KC_TAB)));
    assert(!saw_call("register_code", KC_LALT));
    assert(!saw_name("tap_code16_delay"));

    reset_fixture();
    process_kc_s_0(KC_S_0, &press);
    assert(saw_call("tap_code16", S(KC_0)));
    assert(!saw_call("register_code", KC_LSFT));
}

static void test_jp_toggle_is_physical_intent(void) {
    reset_fixture();
    keyrecord_t press = record(true);
    process_jp_toggle(JP_TOGGLE, &press);
    assert(is_jp_toggle_held());
    assert(saw_call("tap_code", KC_LNG1));

    keyrecord_t release = record(false);
    process_jp_toggle(JP_TOGGLE, &release);
    assert(!is_jp_toggle_held());
    assert(saw_call("tap_code", KC_LNG2));
}

int main(void) {
    test_ctl_space_uses_separated_release();
    test_ctl_space_rapid_repeat_keeps_modifier();
    test_acmd_sp_uses_weak_mods_and_roll_hold();
    test_custom_tap_hold_uses_dynamic_term();
    test_ly_tgml_roll_is_mouse_hold();
    test_ly_tgml_tap_still_enables_mouse_layer();
    test_ly_tgml_does_not_release_external_layer();
    test_alt_tab_and_shift_zero_preserve_modifiers();
    test_jp_toggle_is_physical_intent();
    puts("custom keycode host tests passed");
    return 0;
}
