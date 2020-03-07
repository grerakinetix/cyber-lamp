#include "Constants.h"
#include "Transition.h"

class CenterSlide : public Transition {
    double previousOffset;

  public:
	CenterSlide(Mode *from, Mode *to, uint16_t duration,
	            EasingBase *easingMethod)
	    : Transition(from, to, duration, easingMethod), previousOffset(0) {}

    CRGB const (&getPixels())[HEIGHT][WIDTH] {
        if (to != NULL && millis() - beginTime > duration)
            return to->getPixels();
        return pixels;
    }

	void refresh() {
        if (from != NULL)
            from->refresh();
        if (to != NULL)
            to->refresh();

		uint64_t time = millis() - beginTime;

        // Nothing to do if the transition has ended
        if (time > duration) {
            from = NULL;
            return;
        }

        double easing = easingMethod->easeInOut(time);

        // Source mode
		if (from != NULL) {
			CRGB const(&source)[HEIGHT][WIDTH] = from->getPixels();

			for (uint16_t y = 0; y < HEIGHT; ++y) {
				for (uint16_t x = 0; x < WIDTH; ++x) {
					pixels[y][x] = source[y][x];
					pixels[y][x].fadeToBlackBy(easing);
				}
            }
		}

        // Destination mode
		double offset = easing / 255.0 * (WIDTH / 2 - ((WIDTH % 2 == 0) ? 1 : 0));
        uint16_t leftMargin = WIDTH / 2 - ((WIDTH % 2 == 0) ? 1 : 0) - round(offset);
        uint16_t rightMargin = WIDTH / 2 + round(offset);

		if (to != NULL) {
			CRGB const(&destination)[HEIGHT][WIDTH] = to->getPixels();

			for (uint16_t y = 0; y < HEIGHT; ++y) {
				for (uint16_t x = leftMargin; x <= rightMargin; ++x) {
					pixels[y][x] = destination[y][x];
                    pixels[y][x].fadeToBlackBy(255 - easing);
				}
			}
		} else
            for (uint16_t y = 0; y < HEIGHT; ++y)
				for (uint16_t x = leftMargin; x <= rightMargin; ++x)
					pixels[y][x].setRGB(0, 0, 0);

        // White stripes
        for (uint16_t y = 0; y < HEIGHT; ++y) {
            pixels[y][leftMargin].setRGB(255, 255, 255);
            pixels[y][rightMargin].setRGB(255, 255, 255);
        }

        // White shades
        LinearEase shade;
        shade.setDuration(offset - previousOffset);
        shade.setTotalChangeInPosition(255);
        for (uint16_t y = 0; y < HEIGHT; ++y) {
            for (uint16_t distance = 1; distance <= offset - previousOffset; ++distance) {
#ifdef SCATTERED_SHADE
                uint8_t shadeValue = random8(255 - shade.easeInOut(distance));
#else
                uint8_t shadeValue = 255 - shade.easeInOut(distance);
#endif
                pixels[y][leftMargin + distance].addToRGB(shadeValue);
                pixels[y][rightMargin - distance].addToRGB(shadeValue);
            }
        }

        previousOffset = offset;
	}
};