#pragma once
#include "ButtonsManager.h"
#include "Constants.h"
#include "LEDManager.h"

class Mode;

class Controller {
	ButtonsManager buttonsManager;
	LEDManager ledManager;
	Mode *currentMode;
	ModeID currentModeID;
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