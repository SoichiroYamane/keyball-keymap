#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "../../keyball/lib/keyball/auto_mouse_config.h"

static void test_timeout_round_trip(void) {
    uint16_t encoded = keyball_auto_mouse_timeout_encode(30000);
    assert(encoded == 599);
    assert(keyball_auto_mouse_timeout_decode(encoded, 650) == 30000);
}

static void test_raw_layout_preserves_existing_fields(void) {
    uint32_t raw = 0x00600000u | 0x00000800u | 0x00000700u | 0x0000007fu;
    uint32_t updated = keyball_auto_mouse_timeout_set_raw(
        raw, 30000, KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITH_SCROLLSNAP);

    assert((updated & 0x00600000u) == (raw & 0x00600000u));
    assert((updated & 0x00000800u) == (raw & 0x00000800u));
    assert((updated & 0x00000700u) == (raw & 0x00000700u));
    assert((updated & 0x0000007fu) == (raw & 0x0000007fu));
    assert(((updated >> KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT) &
            KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_MASK) == (599u & 0x1fu));
    assert(((updated >> KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITH_SCROLLSNAP) &
            KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_MASK) == (599u >> 5));
    assert((updated & KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_MASK) != 0);
    assert(keyball_auto_mouse_timeout_from_raw(
               updated, 650, KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITH_SCROLLSNAP) ==
           30000);
}

static void test_legacy_raw_falls_back_to_configured_timeout(void) {
    uint32_t legacy_raw = (23u << KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT);
    assert(keyball_auto_mouse_timeout_from_raw(
               legacy_raw, 30000, KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITH_SCROLLSNAP) ==
           30000);
}

static void test_timeout_is_bounded(void) {
    assert(keyball_auto_mouse_timeout_encode(50) == 0);
    assert(keyball_auto_mouse_timeout_encode(60000) == 1023);
    assert(keyball_auto_mouse_timeout_decode(0, 650) == 650);
    assert(keyball_auto_mouse_timeout_decode(1023, 650) == 51200);
}

int main(void) {
    test_timeout_round_trip();
    test_raw_layout_preserves_existing_fields();
    test_legacy_raw_falls_back_to_configured_timeout();
    test_timeout_is_bounded();
    puts("auto mouse config host tests passed");
    return 0;
}
