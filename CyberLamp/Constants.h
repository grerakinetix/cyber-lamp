#pragma once

// Settings
#define BUTTONS_QUANTITY    5
#define FPS                 60

// Buttons
#define CENTER              0
#define UP                  1
#define DOWN                2
#define RIGHT               3
#define LEFT                4

// Pins
#define LED_PIN             2
#define BUTTON_PINS         {4, 12, 13, 14, 15}        // Center, up, down, right, left

// Matrix
#define WIDTH               16
#define HEIGHT              16
#define LEDS_QUANTITY       WIDTH * HEIGHT
#define BRIGHTNESS_LIMIT    150
#define CURRENT_LIMIT       2000
#define COLOR_ORDER         GRB
#define MATRIX_TYPE         0
#define CONNECTION_ANGLE    0
#define STRIP_DIRECTION     0

// Coordinate system according to configuration
#if (CONNECTION_ANGLE == 0 || CONNECTION_ANGLE == 1)
#define _POSITION x
#else
#define _POSITION (WIDTH - x - 1)
#endif

#if (CONNECTION_ANGLE == 0 || CONNECTION_ANGLE == 3)
#define _ROW y
#else
#define _ROW (HEIGHT - y - 1)
#endif

#if (STRIP_DIRECTION == 0 || STRIP_DIRECTION == 2)
#define LENGTH WIDTH
#define POSITION _POSITION
#define ROW _ROW
#else
#define LENGTH HEIGHT
#define POSITION _ROW
#define ROW _POSITION
#endif

// Modes
typedef enum mode_id {
    WHITE_LIGHT,
    SPARKLES
} ModeID;

// White Light
#define LOWER_TEMP          2000.
#define UPPER_TEMP          4500.

// Transitions
#define SCATTERED_SHADE

// Instructions for controller
typedef enum controller_instructions {
    SWITCH_POWER,
    NEXT_MODE,
    PREVIOUS_MODE,
    INCREASE_BRIGHTNESS,
    DECREASE_BRIGHTNESS,
    INCREASE_SPEED,
    DECREASE_SPEED,
    INCREASE_SCALE,
    DECREASE_SCALE
} Instruction;