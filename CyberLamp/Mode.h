#pragma once
#include <FastLED.h>

#include "Constants.h"

class Mode {
  protected:
	CRGB pixels[HEIGHT][WIDTH];
	uint8_t scale;
	uint8_t speed;

  public:
	Mode(uint8_t scale, uint8_t speed);
	CRGB const (&getPixels())[HEIGHT][WIDTH] { return pixels; };
	virtual void setScale(uint8_t newScale);
	virtual void setSpeed(uint8_t newSpeed);
	virtual void refresh() = 0;
};