#include "LEDManager.h"

#include "Mode.h"
#include "Transition.h"

#include <Arduino.h>

LEDManagerClass::LEDManagerClass() : frameTimeout(millis() + FRAME_DURATION) {
	FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, LEDS_QUANTITY);
#if (CURRENT_LIMIT > 0)
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
#else
	FastLED.setBrightness(BRIGHTNESS_LIMIT);
#endif
	FastLED.clear();
}

inline uint32_t getPixelNumber(uint16_t x, uint16_t y) {
#if (MATRIX_TYPE == 1)
	return row(x, y) * ROW_LENGTH + position(x, y);
#else
	if (row(x, y) % 2 == 0)
		return row(x, y) * ROW_LENGTH + position(x, y);
	return row(x, y) * ROW_LENGTH + ROW_LENGTH - 1 - position(x, y);
#endif
}

void LEDManagerClass::refresh(Mode *mode) {
	if (mode == nullptr)
		return;

	uint64_t time = millis();
	if (time < frameTimeout)
		return;

	frameTimeout = time + FRAME_DURATION;

	mode->refresh();
	auto pixels = mode->getPixels();
	
	for (uint16_t y = 0; y < HEIGHT; ++y)
		for (uint16_t x = 0; x < WIDTH; ++x)
			leds[getPixelNumber(x, y)] = pixels[y][x];

	FastLED.show();
}

void LEDManagerClass::setBrightness(uint8_t brightness) {
	FastLED.setBrightness(brightness);
}

void LEDManagerClass::clear() {
	FastLED.clear(true);
}