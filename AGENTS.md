# AGENTS.md

このファイルはrepository全体の作業規約です。より深い場所の`AGENTS.md`がある場合は、その範囲で具体化されます。system・developer・userの指示が常に優先されます。

## Sources of truth

- `keyball/keyball39/keymaps/yama/`は公開するカスタムKeyball39 keymapの正本です。
- `keyball/`の他のkeyboard、driver、libraryは公式`Yowkees/keyball`由来のvendored sourceです。公式同期と`yama`変更を同じ差分へ無造作に混ぜません。
- QMK build versionは固定した最新安定版`0.33.13`です。変更する場合はIssueで互換性、toolchain、rollbackを先に決めます。
- `.github/workflows/keyball-firmware.yml`はCI buildとtag release CDの正本です。
- `dist/`は過去の生成firmwareを保持する互換pathです。binaryを手編集せず、source、build commit、checksumを確認してから更新します。新しい配布物はGitHub Release artifactを優先します。
- 公式比較の正本は`https://github.com/Yowkees/keyball`の`main`にある`qmk_firmware/keyboards/keyball/`です。release tagと`main`を区別し、比較SHAと日付を記録します。

Repository固有作業では`.agents/skills/keyball-keymap-maintenance/SKILL.md`を使用します。

## Public repository boundary

- Issue、PR、Actions log、Release、artifact、commitは公開情報です。token、credential、端末の絶対path、SSH情報、入力ソースの個人名、個人データを含めません。
- 一つの利用者向け目的を一つのIssueで管理し、purpose、scope、non-goals、受入条件、互換性、Security影響、validation、rollbackを記録します。
- 実装は専用branchと通常PRをIssueへリンクします。force push、checks bypass、未merge branch削除を行いません。
- CI Actionはcommit SHA、containerはdigestへ固定し、通常buildは`contents: read`、tag release jobだけ`contents: write`を使います。

## Hardware and keymap safety

- build、static check、artifact生成は実行できます。実機flash、bootloader、EEPROM reset、OS input-source変更は別の状態変更であり、対象artifactとユーザー意図が明示されない限り実行しません。
- キー配置、tap/hold、language switch、layer、mouse、CPI、scroll、RGB/OLEDの意味を変える前に、物理位置、利用OS、現行挙動、rollbackを確認します。
- `keyball/keyball`のような自己参照linkや、container内絶対pathを指すsymlinkをcommitしません。
- 公式同期で`yama/`を上書きしません。共通driver/library変更では影響する公式keymapもbuildします。

## Language-switch behavior

- base layer右下は`LY_CTL_SPACE`です。押下時間に関係なくrelease時に`Ctrl+Space`を送ります。layer 4への長押し遷移はありません。
- layer 3の`JP_TOGGLE`はpress時に`KC_LNG1`、release時に`KC_LNG2`を送ります。`ENT_IMEVIM`はfirmware内の言語状態を参照します。
- 短押しでも切り替わらない実機症状があるため、100 ms境界だけを原因と断定しません。press/release timing、roll、modifier残留、OS shortcut受理、firmwareとOSの状態ずれを分けて検証します。
- compile成功だけで操作性が確認できたと報告しません。短押し、長押し、roll、連打、英→日、日→英を実機で確認します。

## Workflow

1. `git status --short --branch`、`git remote -v`、`git log -5 --oneline --decorate`を確認します。
2. `keymap.c`、`config.h`、`rules.mk`、custom keycode producer/consumer、共通libraryを読んでから編集します。
3. 公式同期では`yama/`とlocal-only fileを除外してpath単位で比較し、一括置換しません。
4. `apply_patch`で最小変更を行い、無関係なformat、binary、temporary fileを混ぜません。
5. narrow static check、CI相当build、Review 1、staged Review 2の順で検証します。review後の編集は両reviewをやり直します。
6. `v*`tagはreview済みcommitにだけ作成し、CDが作るReleaseの`.hex`と`SHA256SUMS`を確認します。tagやReleaseの作成はユーザーが公開を依頼した場合だけ行います。

## Validation matrix

| 変更範囲 | 必須確認 |
| --- | --- |
| docs・agent・skill | `git diff --check`、参照path、skill validator |
| workflow | YAML parse、`actionlint`、Action SHA、permissions、artifact/release条件 |
| `yama` | QMK 0.33.13で`keyball/keyball39:yama` build、firmware size |
| common driver/library | `yama`と影響する`default`/`via` build、公式差分 |
| language switch | CI buildに加え、実機の短押し・長押し・roll・連打・双方向切り替え |
| release | tag commitとbuild SHA一致、`.hex` checksum、Release asset、書き込み権限の限定 |

## Review checklist

Review 1では物理挙動、layer、OS差分、公式互換性、firmware size、Security、rollbackを確認します。

Review 2ではstaged diff、`git diff --cached --check`、generated binary、symlink、秘密情報、Action pin、CI結果、branch/Issue/PR状態を確認します。skip、既存`dist/`、過去の成功を現在のpassとして扱いません。
