#pragma once
#include "Constants.h"

#include <EasingLibrary.h>
#include <FastLED.h>

class Transition;
class Mode;

class LEDManager {
	CRGB leds[LEDS_QUANTITY];
	uint64_t frameTimeout;

  public:
	LEDManager();
	void refresh(Mode *);
	void setBrightness(uint8_t);
	void clear();
};