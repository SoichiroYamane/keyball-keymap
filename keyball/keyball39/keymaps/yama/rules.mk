RGBLIGHT_ENABLE = no
OLED_ENABLE = yes
VIA_ENABLE = no

RGB_MATRIX_ENABLE = yes

EXTRAKEY_ENABLE = yes # 454 bites
DYNAMIC_TAPPING_TERM_ENABLE = yes # 866 bites
COMBO_ENABLE = yes

# CONSOLE_ENABLE = yes

GRAVE_ESC_ENABLE = no # 0 bites

EXTRAFLAGS += -flto # 0 bites
LTO_ENABLE = yes # 0 bites

# OPTIMIZATION: not working anyway
EXTRAFLAGS += -Os
EXTRAFLAGS += -ffunction-sections -fdata-sections
EXTRAFLAGS += -Wl,--gc-sections
EXTRAFLAGS += -ffast-math # 数値計算の最適化

SRC += libs/init/init.c
SRC += libs/user_keycode.c
SRC += libs/jp_toggle/jp_toggle.c
SRC += libs/tmux/tmux.c
# SRC += libs/leave_ml/leave_ml.c
SRC += libs/hm_tog/hm_tog.c
SRC += libs/layer/layer.c
SRC += libs/led_matrix/led_matrix.c
# SRC += libs/shift_tgml/shift_tgml.c
SRC += libs/ly_tgml/ly_tgml.c
SRC += libs/acmd_sp/acmd_sp.c
SRC += libs/oled/oled.c
# SRC += libs/combo/combo.c
SRC += libs/others/others.c


