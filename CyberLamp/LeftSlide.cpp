#pragma once
#include "Constants.h"
#include "Transition.h"

#include <Arduino.h>

class LeftSlide : public Transition {
	double previousOffset;

  public:
	LeftSlide(Mode *from, Mode *to, uint16_t duration, EasingBase *smoother)
	    : Transition(from, to, duration, smoother), previousOffset(0) {}

	void refresh() {
		if (from != nullptr)
			from->refresh();
		if (to != nullptr)
			to->refresh();

		uint64_t time = millis();

		// Nothing to do if transition has ended
		if (time > timeout) {
			from = nullptr;
			return;
		}

		double value = 255.0 - smoother->easeInOut(timeout - time);
		double offset = value / 255.0 * WIDTH;
		uint16_t halfOffset = round(offset / 2);

		// Source mode
		if (from != nullptr) {
			auto source = from->getPixels();

            for (uint16_t x = round(offset); x < WIDTH; ++x) {
			    for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x] = source[y][x - halfOffset];
					pixels[y][x].fadeToBlackBy(value);
				}
			}
		}

		// Destination mode
		if (to != nullptr) {
			auto destination = to->getPixels();

            for (uint16_t x = 0; x < offset; ++x) {
			    for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x] = destination[y][x - halfOffset + half(WIDTH)];
					pixels[y][x].fadeToBlackBy(255 - value);
				}
			}
		} else {
            for (uint16_t x = 0; x < offset; ++x) {
			    for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x].setColorCode(CRGB::Black);
				}
			}
		}

		// White stripe
		for (uint16_t y = 0; y < HEIGHT; ++y) {
			pixels[y][(uint16_t)offset].setColorCode(CRGB::White);
		}

		// White shade
		LinearEase shade;
		shade.setDuration(offset - previousOffset);
		shade.setTotalChangeInPosition(255);
		for (uint16_t dist = 1; dist <= offset - previousOffset; ++dist) {
			for (uint16_t y = 0; y < HEIGHT; ++y) {
#ifdef SCATTERED_SHADE
				uint8_t shadeValue = random8(255 - shade.easeInOut(dist));
#else
				uint8_t shadeValue = 255 - shade.easeInOut(dist);
#endif
				pixels[y][(uint16_t)offset - dist].addToRGB(shadeValue);
			}
		}

		previousOffset = offset;
	}
};