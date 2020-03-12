#pragma once
#include "Constants.h"

#include <EasingLibrary.h>
#include <FastLED.h>

class Mode;

class LEDManagerClass {
	CRGB leds[LEDS_QUANTITY];
	uint64_t frameTimeout;

  public:
	LEDManagerClass();
	void refresh(Mode *);
	void setBrightness(uint8_t);
	void clear();
};