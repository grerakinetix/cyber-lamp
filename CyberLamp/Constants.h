#pragma once

// Settings
#define BUTTONS_QUANTITY        5
#define DEFAULT_BRIGHTNESS      128
#define DEFAULT_SPEED           128
#define DEFAULT_SCALE           128

// Pins
#define LED_PIN                 2
#define BUTTON_PINS             {4, 12, 13, 14, 15}        // Center, up, down, right, left

// Matrix
#define WIDTH                   16
#define HEIGHT                  16
#define BRIGHTNESS_LIMIT        150
#define CURRENT_LIMIT           2000
#define COLOR_ORDER             GRB
#define MATRIX_TYPE             0
#define CONNECTION_ANGLE        0
#define STRIP_DIRECTION         0

// *** DEVELOPER ZONE ***

// Developer settings
#define FPS                     60

// Buttons
#define CENTER                  0
#define UP                      1
#define DOWN                    2
#define RIGHT                   3
#define LEFT                    4

// Modes
enum ModeID {
    FIRST_MODE = 0,
    WHITE_LIGHT = FIRST_MODE,
    SPARKLES,
    LAST_MODE
};

// White Light
#define LOWER_TEMP              2000.
#define UPPER_TEMP              4500.

// Transitions
#define SCATTERED_SHADE

// EEPROM
#define EEPROM_SIZE             256
#define EEPROM_FLASH_INTERVAL   10000
#define FIRST_BOOT_ADDRESS      200
#define FIRST_BOOT_CHECKSUM     170
#define BRIGHTNESS_ADDRESS      0
#define CURRENT_MODE_ADDRESS    1
#define CURRENT_MODE_DEFAULT    WHITE_LIGHT
#define FIRST_MODE_ADDRESS      2
#define MODE_SIZE               2

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

// Coordinate system according to configuration
#if (CONNECTION_ANGLE == 0 || CONNECTION_ANGLE == 1)
#   define _position(x)         x
#else
#   define _position(x)         (WIDTH - x - 1)
#endif

#if (CONNECTION_ANGLE == 0 || CONNECTION_ANGLE == 3)
#   define _row(y)              y
#else
#   define _row(y)              (HEIGHT - y - 1)
#endif

#if (STRIP_DIRECTION == 0 || STRIP_DIRECTION == 2)
#   define ROW_LENGTH           WIDTH
#   define position(x, y)       _position(x)
#   define row(x, y)            _row(y)
#else
#   define ROW_LENGTH           HEIGHT
#   define position(x, y)       _row(y)
#   define row(x, y)            _position(x)
#endif

// Common expressions
#define LEDS_QUANTITY           WIDTH * HEIGHT
#define FRAME_DURATION          (1000 / FPS)
#define modeAddress(modeID)     FIRST_MODE_ADDRESS + modeID * MODE_SIZE

// Miscellaneous
#define FASTLED_INTERNAL