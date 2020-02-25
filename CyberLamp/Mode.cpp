#include "Mode.h"

Mode::Mode(uint8_t scale, uint8_t speed) : scale(scale), speed(speed) {}

void Mode::setScale(uint8_t newScale) { scale = newScale; }

void Mode::setSpeed(uint8_t newSpeed) { speed = newSpeed; }