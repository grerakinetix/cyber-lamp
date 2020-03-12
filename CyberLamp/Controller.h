#pragma once
#include "ButtonsManager.h"
#include "Constants.h"
#include "LEDManager.h"
#include "SmoothValue.h"

#include <EEPROM.h>

class Mode;

class ControllerClass {
	ButtonsManagerClass ButtonsManager;
	LEDManagerClass LEDManager;
	Mode *mode;
	uint8_t modeID;
	Transition *transition;
	SmoothValue<uint8_t, SineEase> brightness;
	SmoothValue<uint8_t, SineEase> speed;
	SmoothValue<uint8_t, SineEase> scale;
	uint64_t switchTimeout;
	uint64_t eepromTimeout;
	bool power;
	bool poweringOff;

	bool checkEEPROM();
	void restoreState();
	void saveState();
	void parseInstruction(Instruction);

  public:
	ControllerClass();
	void tick();
	void switchPower();
	bool poweredON();
	void nextMode();
	void previousMode();
	void increaseBrightness();
	void decreaseBrightness();
	void increaseSpeed();
	void decreaseSpeed();
	void increaseScale();
	void decreaseScale();
};