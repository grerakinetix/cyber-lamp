#pragma once
#include "Mode.h"

#include <Arduino.h>
#include <EasingLibrary.h>

class Transition : public Mode {
  protected:
    Mode *from, *to;
    EasingBase *easingMethod;
    const uint64_t beginTime;
    const uint16_t duration;

  public:
    Transition(Mode *, Mode *, uint16_t, EasingBase *);
    Mode *getDestinationMode();
    virtual void refresh() = 0;
};