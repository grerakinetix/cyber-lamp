#include "LEDManager.h"
#include "Animation.h"
#include "Mode.h"

LEDManager::LEDManager() {
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, LEDS_QUANTITY);
    if (CURRENT_LIMIT > 0)
        FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    else
        FastLED.setBrightness(BRIGHTNESS_LIMIT);
    FastLED.clear();
    FastLED.show();
}

void LEDManager::refresh(Mode *mode, Animation *animation) {

}