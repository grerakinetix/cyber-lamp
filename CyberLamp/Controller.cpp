#include "Controller.h"

#include "modes.h"

#include <queue>

Mode *initMode(ModeID modeID, uint8_t scale, uint8_t speed) {
	switch (modeID) {
		case WHITE_LIGHT:
			return new WhiteLight(scale, speed);
		case SPARKLES:
			return new Sparkles(scale, speed);
		default:
			return new WhiteLight(scale, speed);
	}
}

Controller::Controller() : brightness(128), speed(128), scale(128) {
	currentModeID = WHITE_LIGHT;
	currentMode =
	    initMode(currentModeID, speed.getSmoothValue(), scale.getSmoothValue());
}

void Controller::parseInstruction(Instruction instruction) {
	switch (instruction) {
		case SWITCH_POWER:
			switchPower();
			break;
		case NEXT_MODE:
			nextMode();
			break;
		case PREVIOUS_MODE:
			previousMode();
			break;
		case INCREASE_BRIGHTNESS:
			increaseBrightness();
			break;
		case DECREASE_BRIGHTNESS:
			decreaseBrightness();
			break;
		case INCREASE_SPEED:
			increaseSpeed();
			break;
		case DECREASE_SPEED:
			decreaseSpeed();
			break;
		case INCREASE_SCALE:
			increaseScale();
			break;
		case DECREASE_SCALE:
			decreaseScale();
			break;
	}
}

void Controller::tick() {
	buttonsManager.tick();

	std::queue<Instruction> &pendingOps = buttonsManager.getPendingOps();
	while (!pendingOps.empty()) {
		parseInstruction(pendingOps.front());
		pendingOps.pop();
	}

	if (switchTimeout && millis() > switchTimeout) {
		power = false;
		switchTimeout = 0;
	}

	if (power) {
		if (brightness.isChanging())
			ledManager.setBrightness(brightness.getSmoothValue());
		if (speed.isChanging())
			currentMode->setSpeed(speed.getSmoothValue());
		if (scale.isChanging())
			currentMode->setScale(scale.getSmoothValue());
		ledManager.refresh(currentMode);
	}
}

void Controller::switchPower() {
	currentMode = (power) ?
	    new CenterSlide(currentMode, nullptr, 1000, new CubicEase()) :
	    new CenterSlide(nullptr,
	                    initMode(currentModeID, scale.getSmoothValue(),
	                             speed.getSmoothValue()),
	                    1000, new CubicEase());

	if (power)
		switchTimeout = millis() + 1000;

	power = true;
}

bool Controller::poweredON() { return power; }

void Controller::nextMode() {}

void Controller::previousMode() {}

void Controller::increaseBrightness() {
	uint8_t temp = brightness.getValue();
	brightness.setValue(constrain(temp * 2, 1, 255));
}

void Controller::decreaseBrightness() {
	uint8_t temp = brightness.getValue();
	brightness.setValue(constrain(temp / 2, 1, 255));
}

void Controller::increaseSpeed() {
	uint8_t temp = speed.getValue();
	speed.setValue(constrain(temp + 16, 1, 255));
}

void Controller::decreaseSpeed() {
	uint8_t temp = speed.getValue();
	speed.setValue(constrain(temp - 16, 1, 255));
}

void Controller::increaseScale() {
	uint8_t temp = scale.getValue();
	scale.setValue(constrain(temp + 16, 1, 255));
}

void Controller::decreaseScale() {
	uint8_t temp = scale.getValue();
	scale.setValue(constrain(temp - 16, 1, 255));
}