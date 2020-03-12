#include "Transition.h"

#include <Arduino.h>

Transition::Transition(Mode *from, Mode *to, uint16_t duration,
                       EasingBase *snoother)
    : Mode(0, 0)
    , from(from)
    , to(to)
    , smoother(smoother)
    , timeout(millis() + duration) {
	this->smoother->setDuration(duration);
	this->smoother->setTotalChangeInPosition(255);
}

CRGB const (&Transition::getPixels())[HEIGHT][WIDTH] {
	if (to != nullptr && millis() > timeout)
		return to->getPixels();
	return pixels;
}

void Transition::setSpeed(uint8_t newSpeed) { to->setSpeed(newSpeed); }

void Transition::setScale(uint8_t newSpeed) { to->setScale(newSpeed); }

Mode *Transition::getSourceMode() { return from; }

Mode *Transition::getDestinationMode() { return to; }