#pragma once
#include "Mode.h"

#include <EasingLibrary.h>

class Transition : public Mode {
  protected:
	Mode *from, *to;
	EasingBase *easingMethod;
	const uint64_t timeout;

  public:
	Transition(Mode *, Mode *, uint16_t, EasingBase *);
	Mode *getDestinationMode();
	virtual void refresh() = 0;
};