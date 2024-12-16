#include "led_matrix.h"

// clang-format off
led_config_t g_led_config = {
    {// Key Matrix to LED Index
     /*
     {14,  10,   6,   3,   0,               43,    40,    37,    34,  30}, \
     {15,  11,   7,   4,   1,               44,    41,    38,    35,  31}, \
     {16,  12,   8,   5,   2,               45,    42,    39,    36,  32}, \
     {17,  13,   9,  21,  22,  23,     24,  25,NO_LED,NO_LED,NO_LED,  33}, \
     //   {18,  19,  20,                      ,    26,    27,    28,  29}  //
     backlights
     */
     {14, 10, 6, 3, 0, 18},
     {15, 11, 7, 4, 1, 19},
     {16, 12, 8, 5, 2, 20},
     {17, 13, 9, 21, 22, 23},
     {30, 34, 37, 40, 43, 26},
     {31, 35, 38, 41, 44, 27},
     {32, 36, 39, 42, 45, 28},
     {33, NO_LED, NO_LED, 29, 25, 24}},
    {
        // LED Index to Physical Position
        // {x,y}: x = 224 / (NUMBER_OF_COLS - 1) * COL_POSITION , y =  64 /
        // (NUMBER_OF_ROWS - 1) * ROW_POSITION
        {60, 0},   // 0
        {60, 21},  // 1
        {60, 43},  // 2
        {45, 0},   // 3
        {45, 21},  // 4
        {45, 43},  // 5
        {30, 0},   // 6
        {30, 21},  // 7
        {30, 43},  // 8
        {30, 64},  // 9
        {15, 0},   // 10
        {15, 21},  // 11
        {15, 43},  // 12
        {15, 64},  // 13
        {0, 0},    // 14
        {0, 21},   // 15
        {0, 43},   // 16
        {0, 64},   // 17
        {45, 0},   // 18
        {0, 0},    // 19
        {0, 43},   // 20
        {30, 43},  // 21
        {60, 64},  // 22
        {75, 64},  // 23
        {149, 64}, // 24
        {164, 64}, // 25
        {187, 43}, // 26
        {224, 43}, // 27
        {224, 0},  // 28
        {179, 0},  // 29
        {224, 0},  // 30
        {224, 21}, // 31
        {224, 43}, // 32
        {224, 64}, // 33
        {209, 0},  // 34
        {209, 21}, // 35
        {209, 43}, // 36
        {194, 0},  // 37
        {194, 21}, // 38
        {194, 43}, // 39
        {179, 0},  // 40
        {179, 21}, // 41
        {179, 43}, // 42
        {164, 0},  // 43
        {164, 21}, // 44
        {164, 43}, // 45
        {0, 0},    // 46
        {0, 0},    // 47
    },
    {
        // LED Index to Flag
        LED_FLAG_KEYLIGHT, // 0
        LED_FLAG_KEYLIGHT, // 1
        LED_FLAG_KEYLIGHT, // 2
        LED_FLAG_KEYLIGHT, // 3
        LED_FLAG_KEYLIGHT, // 4
        LED_FLAG_KEYLIGHT, // 5
        LED_FLAG_KEYLIGHT, // 6
        LED_FLAG_KEYLIGHT, // 7
        LED_FLAG_KEYLIGHT, // 8
        LED_FLAG_KEYLIGHT, // 9
        LED_FLAG_KEYLIGHT, // 10
        LED_FLAG_KEYLIGHT, // 11
        LED_FLAG_KEYLIGHT, // 12
        LED_FLAG_KEYLIGHT, // 13
        LED_FLAG_KEYLIGHT, // 14
        LED_FLAG_KEYLIGHT, // 15
        LED_FLAG_KEYLIGHT, // 16
        LED_FLAG_KEYLIGHT, // 17
        LED_FLAG_KEYLIGHT, // 18
        LED_FLAG_KEYLIGHT, // 19
        LED_FLAG_KEYLIGHT, // 20
        LED_FLAG_KEYLIGHT, // 21
        LED_FLAG_KEYLIGHT, // 22
        LED_FLAG_KEYLIGHT, // 23
        LED_FLAG_KEYLIGHT, // 24
        LED_FLAG_KEYLIGHT, // 25
        LED_FLAG_KEYLIGHT, // 26
        LED_FLAG_KEYLIGHT, // 27
        LED_FLAG_KEYLIGHT, // 28
        LED_FLAG_KEYLIGHT, // 29
        LED_FLAG_KEYLIGHT, // 30
        LED_FLAG_KEYLIGHT, // 31
        LED_FLAG_KEYLIGHT, // 32
        LED_FLAG_KEYLIGHT, // 33
        LED_FLAG_KEYLIGHT, // 34
        LED_FLAG_KEYLIGHT, // 35
        LED_FLAG_KEYLIGHT, // 36
        LED_FLAG_KEYLIGHT, // 37
        LED_FLAG_KEYLIGHT, // 38
        LED_FLAG_KEYLIGHT, // 39
        LED_FLAG_KEYLIGHT, // 40
        LED_FLAG_KEYLIGHT, // 41
        LED_FLAG_KEYLIGHT, // 42
        LED_FLAG_KEYLIGHT, // 43
        LED_FLAG_KEYLIGHT, // 44
        LED_FLAG_KEYLIGHT, // 45
        LED_FLAG_NONE,     // 46
        LED_FLAG_NONE      // 47
    }};
// clang-format on
