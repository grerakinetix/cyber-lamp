#pragma once
#include "Constants.h"
#include "Mode.h"

#include <Arduino.h>
#include <list>

class Sparkles : public Mode {
	std::list<CRGB *> fadingList;

  public:
	Sparkles(uint8_t scale = 50, uint8_t speed = 50) : Mode(scale, speed) {
		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
				pixels[i][j].setRGB(0, 0, 0);
	}

	void refresh() {
		// Removing already black pixels
		for (std::list<CRGB *>::iterator pixel = fadingList.begin();
		     !*(*pixel) && pixel != fadingList.end(); ++pixel)
			fadingList.pop_front();

		// Fading pixels
		for (CRGB *pixel : fadingList) {
			if (pixel->getAverageLight() > 20)
				pixel->fadeToBlackBy(speed);
			else
				pixel->setRGB(0, 0, 0);
		}

		// Creating sparkles
		for (uint8_t i = 0; i < scale; ++i) {
			int y = random(0, HEIGHT);
			int x = random(0, WIDTH);
			if (!pixels[y][x]) {
				pixels[y][x].setHue(random8());
				fadingList.push_back(&pixels[y][x]);
			}
		}
	}
};