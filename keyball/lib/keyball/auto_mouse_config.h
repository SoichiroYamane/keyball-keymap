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

#include <stdint.h>

#define KEYBALL_AUTO_MOUSE_TIMEOUT_QUANTUM 50u
/*
 * amlto's existing low bits are 16..20 in keyball_config_t. Bits 21..22 are
 * scroll-snap when enabled, so the timeout extension uses reserved bits 23..27
 * (or 21..25 when scroll-snap is disabled). Bit 31 marks this format so old
 * truncated values can fall back to the configured keep time after upgrade.
 */
#define KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT 16u
#define KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_MASK 0x1fu
#define KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_MASK 0x1fu
#define KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_BIT 31u
#define KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_MASK \
  ((uint32_t)1u << KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_BIT)
#define KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITH_SCROLLSNAP 23u
#define KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_SHIFT_WITHOUT_SCROLLSNAP 21u
#define KEYBALL_AUTO_MOUSE_TIMEOUT_ENCODED_BITS 10u
#define KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_ENCODED \
  ((1u << KEYBALL_AUTO_MOUSE_TIMEOUT_ENCODED_BITS) - 1u)
#define KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_MS \
  ((KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_ENCODED + 1u) * \
   KEYBALL_AUTO_MOUSE_TIMEOUT_QUANTUM)
#define KEYBALL_AUTO_MOUSE_TIMEOUT_MIN_MS \
  (2u * KEYBALL_AUTO_MOUSE_TIMEOUT_QUANTUM)

static inline uint16_t keyball_auto_mouse_timeout_encode(uint16_t timeout) {
  if (timeout < KEYBALL_AUTO_MOUSE_TIMEOUT_MIN_MS) {
    return 0;
  }

  uint32_t encoded = timeout / KEYBALL_AUTO_MOUSE_TIMEOUT_QUANTUM - 1u;
  if (encoded > KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_ENCODED) {
    encoded = KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_ENCODED;
  }
  return (uint16_t)encoded;
}

static inline uint16_t keyball_auto_mouse_timeout_decode(uint16_t encoded,
                                                          uint16_t fallback) {
  if (encoded == 0) {
    return fallback;
  }
  return (uint16_t)((encoded + 1u) * KEYBALL_AUTO_MOUSE_TIMEOUT_QUANTUM);
}

static inline uint16_t keyball_auto_mouse_timeout_clamp(uint16_t timeout) {
  if (timeout < KEYBALL_AUTO_MOUSE_TIMEOUT_MIN_MS) {
    return KEYBALL_AUTO_MOUSE_TIMEOUT_MIN_MS;
  }
  if (timeout > KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_MS) {
    return KEYBALL_AUTO_MOUSE_TIMEOUT_MAX_MS;
  }
  return timeout;
}

static inline uint32_t keyball_auto_mouse_timeout_set_raw(uint32_t raw,
                                                            uint16_t timeout,
                                                            uint8_t high_shift) {
  uint16_t encoded = keyball_auto_mouse_timeout_encode(timeout);
  uint32_t low_mask = (uint32_t)KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_MASK
                      << KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT;
  uint32_t high_mask = (uint32_t)KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_MASK
                       << high_shift;

  raw &= ~(low_mask | high_mask);
  raw |= ((uint32_t)encoded & KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_MASK)
         << KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT;
  raw |= ((uint32_t)(encoded >> 5) & KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_MASK)
         << high_shift;
  raw |= KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_MASK;
  return raw;
}

static inline uint16_t keyball_auto_mouse_timeout_from_raw(uint32_t raw,
                                                             uint16_t fallback,
                                                             uint8_t high_shift) {
  uint16_t encoded = (uint16_t)((raw >> KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_SHIFT) &
                                KEYBALL_AUTO_MOUSE_TIMEOUT_LOW_MASK);
  if ((raw & KEYBALL_AUTO_MOUSE_TIMEOUT_FORMAT_MASK) == 0) {
    return fallback;
  }
  encoded |= (uint16_t)(((raw >> high_shift) &
                         KEYBALL_AUTO_MOUSE_TIMEOUT_HIGH_MASK)
                        << 5);
  return keyball_auto_mouse_timeout_decode(encoded, fallback);
}
