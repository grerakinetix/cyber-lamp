#pragma once
#include "Constants.h"
#include "Mode.h"

#include <Arduino.h>

class WhiteLight : public Mode {

  private:
	void draw() {
		float temprature =
		    scale * (UPPER_TEMP - LOWER_TEMP) / 10000 + LOWER_TEMP / 100;
		double temp;
		uint8_t red;
		uint8_t green;
		uint8_t blue;

		if (temprature <= 66) {
			red = 255;
			temp = 99.4708025861 * log(temprature) - 161.1195681661;
			temp = round(temp);
			green = constrain(temp, 0, 255);
			if (temprature <= 19)
				blue = 0;
			else {
				temp = 138.5177312231 * log(temprature - 10.) - 305.0447927307;
				temp = round(temp);
				blue = constrain(temp, 0, 255);
			}
		} else {
			temp = 329.698727446 * pow(temprature - 60., -0.1332047592);
			temp = round(temp);
			red = constrain(temp, 0, 255);
			temp = 288.1221695283 * pow(temprature - 60., -0.0755148492);
			temp = round(temp);
			green = constrain(temp, 0, 255);
			blue = 255;
		}

		for (uint16_t i = 0; i < HEIGHT; ++i)
			for (uint16_t j = 0; j < WIDTH; ++j)
				pixels[i][j].setRGB(red, green, blue);
	}

  public:
	WhiteLight(uint8_t speed, uint8_t scale) : Mode(speed, scale) { draw(); }

	void setScale(uint8_t newScale) {
		Mode::setScale(newScale);
		draw();
	}

	void refresh() {}
};