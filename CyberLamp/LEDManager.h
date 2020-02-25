#pragma once
#include <FastLED.h>

#include "Constants.h"

class Animation;
class Mode;

class LEDManager {
    CRGB leds[LEDS_QUANTITY];
    uint8_t brightness = 0;

  public:	
    LEDManager();
    void refresh(Mode *, Animation *);
};