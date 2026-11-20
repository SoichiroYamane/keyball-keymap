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
static uint8_t real_mods;
static uint8_t weak_mods;
static bool a_down;
static bool space_down;

struct report {
    uint16_t time;
    uint8_t mods;
    bool space;
    bool a;
};

static struct report reports[16];
static size_t report_count;

static void reset_fixture(void) {
    report_count = 0;
    real_mods = weak_mods = 0;
    a_down = space_down = false;
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
uint8_t has_anykey(void) { return a_down || space_down; }
uint8_t get_mods(void) { return real_mods; }
uint8_t get_weak_mods(void) { return weak_mods; }
void add_weak_mods(uint8_t mods) { weak_mods |= mods; }

static void send_report(void) {
    const uint8_t mods = real_mods | weak_mods;
    const struct report previous = report_count ? reports[report_count - 1] : (struct report){0};
    if (previous.mods == mods && previous.space == space_down && previous.a == a_down) {
        return;
    }
    assert(report_count < sizeof(reports) / sizeof(reports[0]));
    reports[report_count++] = (struct report){g_now, mods, space_down, a_down};
}

void register_code16(uint16_t code) { record_call("register_code16", code, 0); }
void unregister_code16(uint16_t code) { record_call("unregister_code16", code, 0); }

static void set_key(uint8_t code, bool pressed) {
    switch (code) {
    case KC_A: a_down = pressed; break;
    case KC_SPC: space_down = pressed; break;
    case KC_LCTL:
    case KC_LSFT:
        if (pressed) real_mods |= MOD_BIT(code);
        else real_mods &= ~MOD_BIT(code);
        break;
    default: break;
    }
    send_report();
}

void register_code(uint8_t code) {
    record_call("register_code", code, 0);
    set_key(code, true);
}
void unregister_code(uint8_t code) {
    record_call("unregister_code", code, 0);
    set_key(code, false);
}
void tap_code(uint8_t code) { record_call("tap_code", code, 0); }
void tap_code16(uint16_t code) { record_call("tap_code16", code, 0); }
void tap_code16_delay(uint16_t code, uint16_t delay) {
    record_call("tap_code16_delay", code, delay);
}
void register_weak_mods(uint8_t mods) {
    record_call("register_weak_mods", mods, 0);
    add_weak_mods(mods);
    send_report();
}
void unregister_weak_mods(uint8_t mods) {
    record_call("unregister_weak_mods", mods, 0);
    weak_mods &= ~mods;
    send_report();
}
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

void process_hm_tog(uint16_t keycode, keyrecord_t *r) { (void)keycode; (void)r; }
bool process_tmux_keycodes(uint16_t keycode, keyrecord_t *r) { (void)keycode; (void)r; return true; }
#include "../../keyball/keyball39/keymaps/yama/libs/user_keycode.c"

static void scan_at(uint16_t time) {
    g_now = time;
    matrix_scan_user();
}

static void key_at(uint16_t time, uint16_t code, bool pressed) {
    scan_at(time);
    keyrecord_t r = record(pressed);
    // QMK's action_exec() clears weak mods ahead of the pre-record hook.
    if (pressed) weak_mods = 0;
    if (pre_process_record_user(code, &r) && process_record_user(code, &r) && code < SAFE_RANGE) {
        set_key(code, pressed);
    }
}

static void assert_report(size_t index, uint16_t time, uint8_t mods, bool space, bool a) {
    assert(index < report_count);
    const struct report r = reports[index];
    assert(r.time == time && r.mods == mods && r.space == space && r.a == a);
}

static void test_ctl_space_separates_all_edges(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL);
    const struct { uint16_t press, release; } taps[] = {{0, 25}, {25, 175}, {65380, 65530}};
    for (size_t i = 0; i < sizeof(taps) / sizeof(taps[0]); ++i) {
        reset_fixture();
        const uint16_t release = taps[i].release;
        key_at(taps[i].press, LY_CTL_SPACE, true);
        scan_at(release - 1);
        assert(report_count == 0 && active_layers == 0);
        key_at(release, LY_CTL_SPACE, false);
        assert(report_count == 1);
        assert_report(0, release, ctrl, false, false);
        scan_at(release + 19);
        assert(report_count == 1);
        scan_at(release + 20);
        scan_at(release + 69);
        assert(report_count == 2);
        assert_report(1, release + 20, ctrl, true, false);
        scan_at(release + 70);
        scan_at(release + 89);
        assert(report_count == 3);
        assert_report(2, release + 70, ctrl, false, false);
        scan_at(release + 90);
        assert(report_count == 4 && active_layers == 0);
        assert_report(3, release + 90, 0, false, false);
    }
}

static void test_ctl_space_finishes_before_a_rolled_key(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL);
    const struct { uint16_t arrival, space_down, space_up; } cases[] = {
        {30, 30, 30}, {55, 45, 55}, {96, 45, 95},
    };
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        reset_fixture();
        key_at(0, LY_CTL_SPACE, true);
        key_at(25, LY_CTL_SPACE, false);
        if (cases[i].arrival >= 45) scan_at(45);
        if (cases[i].arrival >= 95) scan_at(95);
        key_at(cases[i].arrival, KC_A, true);
        assert(report_count == 5 && g_now == cases[i].arrival);
        assert_report(1, cases[i].space_down, ctrl, true, false);
        assert_report(2, cases[i].space_up, ctrl, false, false);
        assert_report(3, cases[i].arrival, 0, false, false);
        assert_report(4, cases[i].arrival, 0, false, true);
        key_at(cases[i].arrival + 8, KC_A, false);
        assert_report(5, cases[i].arrival + 8, 0, false, false);
    }
}

static void test_ctl_space_rapid_repeat_has_two_complete_chords(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL);
    reset_fixture();
    key_at(0, LY_CTL_SPACE, true);
    key_at(25, LY_CTL_SPACE, false);
    scan_at(45);
    key_at(46, LY_CTL_SPACE, true);
    assert(report_count == 4);
    assert_report(2, 46, ctrl, false, false);
    assert_report(3, 46, 0, false, false);
    key_at(75, LY_CTL_SPACE, false);
    scan_at(95);
    scan_at(145);
    scan_at(165);
    assert(report_count == 8);
    assert_report(4, 75, ctrl, false, false);
    assert_report(5, 95, ctrl, true, false);
    assert_report(6, 145, ctrl, false, false);
    assert_report(7, 165, 0, false, false);
}

static void test_ctl_space_preserves_a_held_space_and_shift(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL), shift = MOD_BIT(KC_LSFT);
    reset_fixture();
    key_at(0, KC_LSFT, true);
    key_at(10, KC_SPC, true);
    key_at(20, LY_CTL_SPACE, true);
    key_at(25, LY_CTL_SPACE, false);
    key_at(30, KC_SPC, false);
    scan_at(45);
    assert(report_count == 3);
    assert_report(1, 10, shift, true, false);
    assert_report(2, 30, shift, false, false);
    key_at(50, KC_LSFT, false);
    scan_at(51);
    scan_at(71);
    scan_at(121);
    scan_at(141);
    assert(report_count == 8);
    assert_report(3, 50, 0, false, false);
    assert_report(4, 51, ctrl, false, false);
    assert_report(5, 71, ctrl, true, false);
    assert_report(6, 121, ctrl, false, false);
    assert_report(7, 141, 0, false, false);
}

static void test_ctl_space_preserves_a_held_ctrl(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL);
    reset_fixture();
    key_at(0, KC_LCTL, true);
    key_at(10, LY_CTL_SPACE, true);
    key_at(25, LY_CTL_SPACE, false);
    scan_at(45);
    scan_at(95);
    scan_at(115);
    assert(report_count == 3 && real_mods == ctrl && weak_mods == 0);
    assert_report(1, 45, ctrl, true, false);
    assert_report(2, 95, ctrl, false, false);
    key_at(125, KC_LCTL, false);
    assert_report(3, 125, 0, false, false);
}

static void test_ctl_space_counts_two_fresh_taps_while_shift_is_held(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL), shift = MOD_BIT(KC_LSFT);
    reset_fixture();
    key_at(0, KC_LSFT, true);
    key_at(10, LY_CTL_SPACE, true);
    key_at(20, LY_CTL_SPACE, false);
    key_at(30, LY_CTL_SPACE, true);
    key_at(40, LY_CTL_SPACE, false);
    assert(report_count == 1);
    assert_report(0, 0, shift, false, false);
    key_at(50, KC_LSFT, false);
    for (uint16_t now = 51; now <= 260; ++now) scan_at(now);
    assert(report_count == 10);
    assert_report(3, 71, ctrl, true, false);
    assert_report(5, 141, 0, false, false);
    assert(reports[7].time > reports[5].time);
    assert_report(7, reports[7].time, ctrl, true, false);
    assert_report(9, reports[9].time, 0, false, false);
}

static void test_ctl_space_discards_stale_requests_and_prioritizes_new_typing(void) {
    const uint8_t shift = MOD_BIT(KC_LSFT);
    for (unsigned i = 0; i < 2; ++i) {
        reset_fixture();
        key_at(0, KC_LSFT, true);
        key_at(10, LY_CTL_SPACE, true);
        key_at(20, LY_CTL_SPACE, false);
        if (i) {
            key_at(25, KC_A, true);
            assert_report(1, 25, shift, false, true);
            key_at(35, KC_A, false);
        }
        const uint16_t release = i ? 45 : 350;
        key_at(release, KC_LSFT, false);
        for (uint16_t now = release + 1; now <= release + 120; ++now) scan_at(now);
        assert(report_count == (i ? 4u : 2u));
    }
}

static void test_ctl_space_preserves_request_during_buffered_press(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL), shift = MOD_BIT(KC_LSFT);
    reset_fixture();
    key_at(0, KC_LSFT, true);
    key_at(10, LY_CTL_SPACE, true);
    scan_at(15);
    keyrecord_t buffered = record(true);
    assert(pre_process_record_user(KC_LCTL, &buffered));
    key_at(20, LY_CTL_SPACE, false);
    scan_at(115);
    assert(process_record_user(KC_LCTL, &buffered));
    set_key(KC_LCTL, true);
    assert_report(1, 115, shift | ctrl, false, false);
    key_at(125, KC_LCTL, false);
    key_at(130, KC_LSFT, false);
    scan_at(131);
    scan_at(151);
    scan_at(201);
    scan_at(221);
    assert(report_count == 8);
    assert_report(5, 151, ctrl, true, false);
    assert_report(7, 221, 0, false, false);
}

static void test_acmd_timeout_does_not_combine_with_language_chord(void) {
    const uint8_t ctrl = MOD_BIT(KC_LCTL);
    const uint8_t alt_gui = MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI);
    reset_fixture();
    key_at(0, LY_CTL_SPACE, true);
    key_at(5, LY_TGML, true);
    scan_at(105);
    assert(layer_state_is(2));
    key_at(110, ACMD_SP, true);
    key_at(200, LY_CTL_SPACE, false);
    scan_at(210);
    assert(g_now == 210 && report_count == 5);
    assert_report(0, 200, ctrl, false, false);
    assert_report(1, 210, ctrl, true, false);
    assert_report(2, 210, ctrl, false, false);
    assert_report(3, 210, 0, false, false);
    assert_report(4, 210, alt_gui, false, false);
    key_at(240, ACMD_SP, false);
    key_at(241, LY_TGML, false);
    assert(active_layers == 0);
    assert_report(5, 240, 0, false, false);
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
    test_ctl_space_separates_all_edges();
    test_ctl_space_finishes_before_a_rolled_key();
    test_ctl_space_rapid_repeat_has_two_complete_chords();
    test_ctl_space_preserves_a_held_space_and_shift();
    test_ctl_space_preserves_a_held_ctrl();
    test_ctl_space_counts_two_fresh_taps_while_shift_is_held();
    test_ctl_space_discards_stale_requests_and_prioritizes_new_typing();
    test_ctl_space_preserves_request_during_buffered_press();
    test_acmd_timeout_does_not_combine_with_language_chord();
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
