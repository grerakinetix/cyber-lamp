#include "LEDManager.h"

#include "Mode.h"
#include "Transition.h"

class LEDManager::BrightnessManager {
    bool active;
	uint8_t lastBrightness;
	uint8_t newBrightness;
	LinearEase easingMethod;
	uint64_t beginTime;
	uint8_t duration;

	BrightnessManager();
	void refresh();
	void setBrightness(uint8_t, uint16_t = 1000);
};

LEDManager::BrightnessManager::BrightnessManager() : beginTime(millis()), duration(0) {}

void LEDManager::BrightnessManager::refresh() {
    if (!active)
        return;

    uint64_t time = millis() - beginTime;

    if (time > duration) {
        FastLED.setBrightness(newBrightness);
        active = false;
        return;
    }

    FastLED.setBrightness(lastBrightness + easingMethod.easeInOut(time));
}

void LEDManager::BrightnessManager::setBrightness(uint8_t brightness, uint16_t newDuration) {
    active = true;
    lastBrightness = FastLED.getBrightness();
    newBrightness = brightness;
    easingMethod.setDuration(newDuration);
    easingMethod.setTotalChangeInPosition(newBrightness - lastBrightness);
    beginTime = millis();
    duration = newDuration;
}

LEDManager::LEDManager() {
	FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, LEDS_QUANTITY);
#if (CURRENT_LIMIT > 0)
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
#else
	FastLED.setBrightness(BRIGHTNESS_LIMIT);
#endif
	FastLED.clear();
	FastLED.show();
}

inline uint32_t getPixelNumber(uint16_t x, uint16_t y) {
#if (MATRIX_TYPE == 1)
	return ROW * LENGTH + POSITION;
#else
	if (ROW % 2 == 0)
		return ROW * LENGTH + POSITION;
	return ROW * LENGTH + LENGTH - POSITION - 1;
#endif
}

void LEDManager::refresh(Mode *mode) {
	CRGB const(&pixels)[HEIGHT][WIDTH] = mode->getPixels();
	for (uint16_t y = 0; y < HEIGHT; ++y)
		for (uint16_t x = 0; x < WIDTH; ++x)
			leds[getPixelNumber(x, y)] = pixels[y][x];
}

void LEDManager::setBrightness(uint8_t brightness, uint16_t duration) {
	lastBrightness = FastLED.getBrightness();
	newBrightness = brightness;
	brightnessChanger = new CubicEase();
	brightnessChanger->setDuration(duration);
	brightnessChanger->setTotalChangeInPosition(brightness - lastBrightness);
}