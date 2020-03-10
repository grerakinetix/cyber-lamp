#pragma once
#include "ButtonsManager.h"
#include "Constants.h"
#include "LEDManager.h"
#include "SmoothValue.h"

class Mode;

class Controller {
	ButtonsManager buttonsManager;
	LEDManager ledManager;
	Mode *currentMode;
	ModeID currentModeID;
	SmoothValue<uint8_t, SineEase> brightness;
	SmoothValue<uint8_t, SineEase> speed;
	SmoothValue<uint8_t, SineEase> scale;
	uint64_t switchTimeout;
	bool power = false;

	void parseInstruction(Instruction);

  public:
	Controller();
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