#include "Transition.h"

#include <Arduino.h>

Transition::Transition(Mode *from, Mode *to, uint16_t duration,
                       EasingBase *easingMethod)
    : Mode(0, 0)
    , from(from)
    , to(to)
    , beginTime(millis())
    , duration(duration)
    , easingMethod(easingMethod) {
	easingMethod->setDuration(duration);
	easingMethod->setTotalChangeInPosition(255);
}

Mode *Transition::getDestinationMode() { return to; }