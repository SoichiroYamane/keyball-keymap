# keyball-keymap

[![Build and release Keyball firmware](https://github.com/SoichiroYamane/keyball-keymap/actions/workflows/keyball-firmware.yml/badge.svg)](https://github.com/SoichiroYamane/keyball-keymap/actions/workflows/keyball-firmware.yml)

This repository is yama's keymap for [keyball 39 🎱](https://github.com/Yowkees/keyball).

> [!warning]
> US配列のキーボード設定です。

## 📝 日本語での記事

[Dockerで始めるkeyball用QMK](https://zenn.dev/soichiro_yamane/articles/qmk-docker-keyball-init)

## Firmware

GitHub Actions builds the `yama` keymap and a `via` baseline with QMK 0.33.13. Each successful run provides `.hex` files and `SHA256SUMS` as a downloadable artifact.

Pushing a reviewed `v*` tag creates a GitHub Release containing the `yama` firmware and its checksum. The workflow only builds and publishes files; it does not flash a keyboard.

Keyball39 is a split keyboard with one controller on each half. For a local
update, verify the checksum and flash the same `yama` hex to both controllers,
connecting each half to USB in turn. Updating only the USB-connected half can
mix QMK versions and cause key positions, layer keys, or trackball direction
to appear incorrect. The Keyball39 source also pins the split-matrix polarity
needed by QMK 0.33.13 to preserve the established physical orientation.
