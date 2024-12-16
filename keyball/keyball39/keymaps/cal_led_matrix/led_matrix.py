import os
import math
import pandas as pd
import numpy as np

file_path = "./led_matrix.csv"
df = pd.read_csv(file_path, header=None)

# Convert DataFrame to list
df_list = df.values.tolist()

# Get size of the matrix
len_rows = len(df_list) - 1  # Exclude the last row: backlights
len_cols = len(df_list[0])

# Get maximum value in the df_list
max_val = int(max([max(row) for row in df_list]))


def get_led_position(row: int, col: int) -> tuple[int, int]:
    QMK_MAX_WIDTH = 224
    QMK_MAX_HEIGHT = 64

    # Calculate the position of the LED
    _x = (QMK_MAX_WIDTH / (len_cols - 1)) * col
    _y = (QMK_MAX_HEIGHT / (len_rows - 1)) * row

    x = int(round(_x))
    y = int(round(_y))
    return x, y


output_led_position = "led_position.txt"
output_led_status = "led_status.txt"

LED_MAX_NUM_ONE_HAND = 24
LED_MAX_NUM = LED_MAX_NUM_ONE_HAND * 2


def is_nan(item):
    return item == "nan" or math.isnan(item)


def create_string_led_position(x, y, target):
    return "{{{},{}}}".format(x, y).ljust(17) + f", //{target}"


def create_string_led_status(target, bool_val):
    if bool_val:
        return "LED_FLAG_KEYLIGHT".ljust(17) + f", //{target}"
    else:
        return "LED_FLAG_NONE".ljust(17) + f", //{target}"


def write_and_print(f, string):
    f.write(string + "\n")
    print(string)


# If outputs exist, delete them
if os.path.exists(output_led_position):
    os.remove(output_led_position)
if os.path.exists(output_led_status):
    os.remove(output_led_status)


target_list = list(range(0, LED_MAX_NUM))
with open(output_led_position, "a") as f:
    with open(output_led_status, "a") as f_status:
        for target in target_list:
            if target > max_val:
                x, y = 0, 0
                led_position = create_string_led_position(x, y, target)
                write_and_print(f, led_position)

                is_led = False
                led_status = create_string_led_status(target, is_led)
                write_and_print(f_status, led_status)

            for row, row_items in enumerate(df_list):
                for col, item in enumerate(row_items):
                    if is_nan(item):
                        continue

                    item = int(item)
                    if item == target:
                        if row == 4:  # Handle back light
                            x, y = 0, 0
                            is_led = False
                        else:
                            x, y = get_led_position(row, col)
                            is_led = True

                        led_position = create_string_led_position(x, y, target)
                        write_and_print(f, led_position)

                        led_status = create_string_led_status(target, is_led)
                        write_and_print(f_status, led_status)
