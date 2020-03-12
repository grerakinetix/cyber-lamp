#include "Mode.h"

Mode::Mode(uint8_t speed, uint8_t scale) : speed(speed), scale(scale) {}

CRGB const (&Mode::getPixels())[HEIGHT][WIDTH] { return pixels; }

void Mode::setSpeed(uint8_t newSpeed) { speed = newSpeed; }

void Mode::setScale(uint8_t newScale) { scale = newScale; }