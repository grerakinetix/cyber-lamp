#pragma once
#include "Constants.h"
#include "Transition.h"

#include <Arduino.h>

class CenterSlide : public Transition {
	double previousOffset;

  public:
	CenterSlide(Mode *from, Mode *to, uint16_t duration, EasingBase *smoother)
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

		// Source mode
		if (from != nullptr) {
			auto source = from->getPixels();

			for (uint16_t x = 0; x < WIDTH; ++x) {
				for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x] = source[y][x];
					pixels[y][x].fadeToBlackBy(value);
				}
			}
		}

		// Destination mode
		double offset = value / 255.0 * half(WIDTH);
		uint16_t leftMargin = half(WIDTH) - round(offset);
		uint16_t rightMargin = WIDTH / 2 + round(offset);

		if (to != nullptr) {
			auto destination = to->getPixels();

			for (uint16_t x = leftMargin; x <= rightMargin; ++x) {
				for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x] = destination[y][x];
					pixels[y][x].fadeToBlackBy(255 - value);
				}
			}
		} else {
			for (uint16_t x = leftMargin; x <= rightMargin; ++x) {
				for (uint16_t y = 0; y < HEIGHT; ++y) {
					pixels[y][x].setColorCode(CRGB::Black);
				}
			}
		}

		// White stripes
		for (uint16_t y = 0; y < HEIGHT; ++y) {
			pixels[y][leftMargin].setColorCode(CRGB::White);
			pixels[y][rightMargin].setColorCode(CRGB::White);
		}

		// White shades
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
				pixels[y][leftMargin + dist].addToRGB(shadeValue);
				pixels[y][rightMargin - dist].addToRGB(shadeValue);
			}
		}

		previousOffset = offset;
	}
};