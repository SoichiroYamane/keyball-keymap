---
name: keyball-keymap-maintenance
description: Review and maintain this public Keyball39 yama QMK keymap repository, including custom key behavior, vendored Keyball sources, QMK 0.22.14 builds, CI artifacts, tag-based GitHub Releases, upstream comparison, and public Issue/PR evidence. Use when changing or validating files under keyball/, investigating language switching or tap/hold behavior, updating dist firmware, comparing with Yowkees/keyball, or editing GitHub Actions and release automation. Do not treat this skill as authorization to flash hardware, change OS input settings, publish a tag or Release, overwrite yama, or disclose local machine information.
---

# Keyball Keymap Maintenance

Read root `AGENTS.md` before acting. It defines the public-information boundary, hardware authority gates, and release rules.

## Repository map

- `keyball/keyball39/keymaps/yama/`: source of truth for the custom keymap.
- `keyball/drivers/` and `keyball/lib/`: vendored pointing-device and shared Keyball behavior.
- Other `keyball*/keymaps/{default,via,test}` paths: upstream baselines used for compatibility checks.
- `.github/workflows/keyball-firmware.yml`: QMK 0.22.14 build, artifact, and tag Release automation.
- `dist/`: legacy tracked firmware snapshot; never hand-edit or treat it as proof of the current source build.

## Triage and inspection

Run:

```console
git status --short --branch
git remote -v
git log -5 --oneline --decorate
```

Identify the physical key, layer, OS, QMK version, affected custom handlers, artifact contract, public Issue, and whether any operation can flash hardware or publish a Release.

Read `keymap.c`, keymap `config.h` and `rules.mk`, the keyboard definition, every custom keycode handler and consumer, and relevant common libraries. Follow `process_record_user`, `matrix_scan_user`, layer hooks, and state variables end to end.

For language switching, inspect `LY_CTL_SPACE`, `JP_TOGGLE`, `ENT_IMEVIM`, `TAPPING_TERM`, `HOLD_ON_OTHER_KEY_PRESS`, press/release timing, roll behavior, and modifier cleanup. Do not infer OS input state from a firmware boolean; the OS can change it independently.

## Compare upstream safely

Use only `Yowkees/keyball` as upstream. Record comparison date, `main` SHA, and changed paths. Distinguish upstream changes, `yama` customization, local utilities, generated firmware, and invalid symlinks.

Use a sparse temporary checkout when necessary:

```console
git clone --depth 1 --filter=blob:none --sparse \
  https://github.com/Yowkees/keyball.git <temporary-directory>
git -C <temporary-directory> sparse-checkout set \
  qmk_firmware/keyboards/keyball
diff -qr --exclude=yama \
  keyball \
  <temporary-directory>/qmk_firmware/keyboards/keyball \
  | sed '\#^Only in keyball: keyball$#d'
```

Inspect textual diffs before syncing. Never replace the whole tree or overwrite `yama/`.

## Implement and track work

Use `apply_patch`. Keep source, handler registration, configuration, and tests in one coherent change. Prefer QMK standard tap/hold facilities when they match the required timing, but preserve physical layout unless the user approves a behavior change.

Use one public Issue for one durable user goal. Include purpose, observed behavior, scope, non-goals, acceptance, compatibility, Security, validation, and rollback. Link a normal PR with `Refs #N`; keep its body to current outcome, checks, blocker, residual risk, and next work.

Never publish credentials, machine paths, private input-source names, or personal data. Issue/PR updates, tag creation, and Release publication are external writes and require user scope. Do not force-push or bypass failed checks.

## CI and CD contract

Keep QMK at `0.22.14` unless an Issue explicitly changes it. Pin GitHub Actions by commit SHA and the QMK CLI container by digest. Use read-only token permission for builds. Allow `contents: write` only in the tag Release job.

CI must build `keyball/keyball39:yama` and a `via` baseline, produce deterministic filenames, create `SHA256SUMS`, and upload artifacts. CD must run only for reviewed `v*` tags, download the artifact from the same workflow run, and attach the `yama` `.hex` plus checksum to a GitHub Release. It must never flash hardware or commit generated binaries.

## Validate and review

Run static checks:

```console
git diff --check
python3 -m json.tool keyball/keyball39/info.json >/dev/null
actionlint .github/workflows/keyball-firmware.yml
```

Use the workflow's pinned QMK environment for firmware compilation. Check the ATmega32U4 firmware size. Existing `dist/*.hex` is not current build evidence.

For language behavior, require manual verification after an explicitly authorized flash: short taps, holds, rolls, rapid repeats, layer recovery, English-to-Japanese, Japanese-to-English, and no stuck modifiers. A CI pass cannot replace this lane.

Perform Review 1 on full behavior, upstream compatibility, Security, public content, and rollback. Stage only intended files, then perform Review 2 on the staged diff, generated files, symlinks, secrets, Action pins, permissions, exact CI head, and Release conditions. Any edit restarts both reviews and affected checks.
