/*
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#pragma once

#include QMK_KEYBOARD_H

#include <stdint.h>
#include <stdlib.h>

// Keep the original speed curve in tenths, but never turn a nonzero axis
// into a zero report. This avoids a dead zone without floating-point code.
static inline uint8_t keyball_mouse_speed_factor(uint16_t movement_size) {
  if (movement_size > 60) return 30;
  if (movement_size > 30) return 15;
  if (movement_size > 5) return 10;
  if (movement_size > 4) return 9;
  if (movement_size > 3) return 7;
  if (movement_size > 2) return 5;
  if (movement_size > 1) return 2;
  return 10;
}

static inline uint16_t keyball_mouse_movement_size(
    const report_mouse_t *report) {
  return (uint16_t)abs((int)report->x) +
         (uint16_t)abs((int)report->y);
}

static inline int8_t keyball_mouse_speed_scale_axis(int8_t value,
                                                     uint8_t factor) {
  int16_t magnitude = (int16_t)abs((int)value) * factor;
  if (magnitude == 0) return 0;
  magnitude = (magnitude + 9) / 10;
  if (magnitude > 127) magnitude = 127;
  return value < 0 ? -magnitude : magnitude;
}

static inline void keyball_mouse_speed_apply(report_mouse_t *report) {
  uint16_t movement_size = keyball_mouse_movement_size(report);
  uint8_t factor = keyball_mouse_speed_factor(movement_size);

  report->x = keyball_mouse_speed_scale_axis(report->x, factor);
  report->y = keyball_mouse_speed_scale_axis(report->y, factor);
}
