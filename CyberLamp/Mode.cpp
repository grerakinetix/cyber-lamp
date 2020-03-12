#include "Mode.h"

Mode::Mode(uint8_t scale, uint8_t speed) : scale(scale), speed(speed) {}

CRGB const (&Mode::getPixels())[HEIGHT][WIDTH] { return pixels; }

void Mode::setSpeed(uint8_t newSpeed) { speed = newSpeed; }

void Mode::setScale(uint8_t newScale) { scale = newScale; }