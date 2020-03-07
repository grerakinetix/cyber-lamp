#pragma once
#include "Constants.h"

#include <EasingLibrary.h>
#include <FastLED.h>

class Transition;
class Mode;

class LEDManager {
	class BrightnessManager;

	CRGB leds[LEDS_QUANTITY];
	BrightnessManager *brightnessManager;

  public:
	LEDManager();
	void refresh(Mode *);
	void setBrightness(uint8_t, uint16_t = 1000);
};