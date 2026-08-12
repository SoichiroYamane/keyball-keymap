# Upstream Keyball audit

Comparison date: 2026-08-12 (Asia/Tokyo)

The comparison target is the `main` branch of [Yowkees/keyball](https://github.com/Yowkees/keyball), not a release tag.

| Source | Revision |
| --- | --- |
| Upstream `main` | [`78de67c49f38836aca06bccd87b42d297d89e1b4`](https://github.com/Yowkees/keyball/commit/78de67c49f38836aca06bccd87b42d297d89e1b4) |
| Upstream commit date | 2026-07-22 11:06:54 +09:00 |
| Local audit base | `9e28232bf61881998a6f5e659f12147936cd75c0` |
| QMK build contract | 0.33.13 (`332fa30e173e5b0ecc0c70ff166974b6db86525e`) |

The local `keyball/` tree was compared path by path with upstream
`qmk_firmware/keyboards/keyball/`. The `yama` keymap, calibration data, and
custom yama libraries are intentionally excluded from upstream replacement.

## Applied upstream changes

These are narrow, behavior-scoped changes from upstream and retain the local
QMK 0.33.13 adaptations:

| Path | Upstream reference | Decision and compatibility note |
| --- | --- | --- |
| `keyball/lib/keyball/keyball.c` | [`11802b6`](https://github.com/Yowkees/keyball/commit/11802b6a2a7c1091c5f7e754ce744978ccd4936) | Added the missing `OLED_ENABLE` guard around `pressing_keys_update()`. The existing local OLED guard for `LFSTR_*` was already equivalent. |
| `keyball/lib/keyball/README.md` | [`c345c4f`](https://github.com/Yowkees/keyball/commit/c345c4f726874ec73243bde6fc6b91af4a76b40f) | Linked the English and Japanese special-keycode sections. No firmware behavior changes. |

## Deferred upstream changes

| Path | Upstream reference | Decision and reason |
| --- | --- | --- |
| `keyball/drivers/pmw3360/pmw3360.c` | [`621b503`](https://github.com/Yowkees/keyball/commit/621b50367749deb6b98e61e43a95f3863c42084a) | The non-AVR divisor belongs with the RP2040 conversion, but no current Keyball44 build is valid under the repository's fixed QMK 0.33.13 contract. Keep the local QMK 0.33 GPIO adaptation until the separate board migration is completed. |
| `keyball/keyball44/config.h` | [`621b503`](https://github.com/Yowkees/keyball/commit/621b50367749deb6b98e61e43a95f3863c42084a) | RP2040 SPI and split-polarity definitions are deferred with the board migration. |
| `keyball/keyball44/readme.md` | [`621b503`](https://github.com/Yowkees/keyball/commit/621b50367749deb6b98e61e43a95f3863c42084a) | The RP2040 build command is deferred until the corresponding source is buildable under QMK 0.33.13. |

The RP2040 change was tested in a temporary source copy with `info.json`
renamed to `keyboard.json`; Keyball44 `default` still failed on its
pre-existing obsolete `KC_BTN*`/`RGB_*` keycodes. No Keyball44 source is
changed in this PR.

## Retained local compatibility or behavior

The following differences are deliberately not replaced with upstream's
current files:

- `keyball/keyball39/config.h`: QMK 0.33.13 `RGBLIGHT_LED_COUNT` and
  `SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT` preserve the tested physical split
  orientation and RGB build.
- `keyball/keyball39/keyboard.json`: this is the local QMK 0.33.13-compatible
  rename of upstream's otherwise identical `info.json`.
- `keyball/keyball39/keymaps/{default,develop,via}/`: local QMK 0.33.13
  keycode migrations (`KC_BTN*`, `RGB_*`) and the existing VIA layer behavior
  are retained. Replacing them would reintroduce obsolete keycodes or remove
  the local auto-mouse handling.
- `keyball/keyball39/keymaps/via/config.h`: the local dynamic layer count and
  auto-mouse keep-time contract are retained; upstream's static-gradient
  choice is not a compatible default for this repository.
- `keyball/lib/keyball/keyball.c` and `.h`: local QMK callback signatures,
  mouse-key handling, split/trackball transforms, movement scaling, and
  auto-mouse timeout behavior are retained. The full upstream file would
  discard these local patches.
- `keyball/keyball39/keyball39.h`: the include-order-only difference is
  retained because the local QMK build is already validated.
- `keyball/readme.md`: local documentation remains pinned to QMK 0.33.13;
  upstream's 0.22.14 instructions are stale for this repository.

## Local-only paths

`keyball/keyball39/keymaps/yama/`, its custom libraries, and
`keyball/keyball39/keymaps/cal_led_matrix/` are local sources of truth and are
not upstream synchronization targets.

## Validation and rollback

The PR validates Keyball39 `yama` and `via` with QMK 0.33.13. The attempted
Keyball44 `default` validation is recorded as blocked by its pre-existing
QMK 0.33.13 metadata/keycode migration gap; no Keyball44 source is changed in
this PR. It also runs static checks and the existing CI security/build checks.
No hardware flash, EEPROM reset, OS input-source change, tag, or Release is
part of this audit.

Rollback is a normal revert of the commits in PR #9, restoring the preceding
vendored baseline `9e28232bf61881998a6f5e659f12147936cd75c0`; generated
firmware is not tracked in this audit.
