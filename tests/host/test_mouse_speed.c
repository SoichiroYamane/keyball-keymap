#include <assert.h>
#include <stdio.h>

#define QMK_KEYBOARD_H "qmk_test_stub.h"
#include "qmk_test_stub.h"
#include "../../keyball/lib/keyball/mouse_speed.h"

static void test_small_motion_is_not_lost(void) {
    report_mouse_t report = {.x = 2, .y = 0};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 1);

    report = (report_mouse_t){.x = 2, .y = 0};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 1);

    report = (report_mouse_t){.x = 2, .y = 0};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 1);
}

static void test_diagonal_motion_preserves_each_axis(void) {
    report_mouse_t report = {.x = 1, .y = 1};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 1);
    assert(report.y == 1);

    for (int i = 0; i < 3; i++) {
        report = (report_mouse_t){.x = 1, .y = 1};
        keyball_mouse_speed_apply(&report);
        assert(report.x == 1);
        assert(report.y == 1);
    }

    report = (report_mouse_t){.x = 1, .y = 1};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 1);
    assert(report.y == 1);
}

static void test_large_motion_keeps_acceleration_and_clips(void) {
    report_mouse_t report = {.x = 61, .y = 0};
    keyball_mouse_speed_apply(&report);
    assert(report.x == 127);
    assert(report.y == 0);
}

int main(void) {
    test_small_motion_is_not_lost();
    test_diagonal_motion_preserves_each_axis();
    test_large_motion_keeps_acceleration_and_clips();
    puts("mouse speed host tests passed");
    return 0;
}
