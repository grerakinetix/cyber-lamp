#pragma once
#include "Constants.h"

#include <EasingLibrary.h>
#include <FastLED.h>

class Transition;
class Mode;

class LEDManager {
	CRGB leds[LEDS_QUANTITY];

  public:
	LEDManager();
	void refresh(Mode *);
	void setBrightness(uint8_t);
};