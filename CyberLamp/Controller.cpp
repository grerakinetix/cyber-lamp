#include "Controller.h"

#include "modes.h"

#include <queue>

Mode *initMode(uint8_t modeID, uint8_t scale, uint8_t speed) {
	switch (modeID) {
		case WHITE_LIGHT:
			return new WhiteLight(scale, speed);
		case SPARKLES:
			return new Sparkles(scale, speed);
		default:
			return new WhiteLight(scale, speed);
	}
}

Controller::Controller()
    : mode(nullptr)
    , modeID(WHITE_LIGHT)
    , transition(nullptr)
    , brightness(128)
    , speed(128)
    , scale(128)
    , switchTimeout(0)
    , power(false)
    , poweringOff(false) {
	// Read values from memory
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

	// Checking whether transition has finished
	if (transition != nullptr && millis() > switchTimeout) {
		mode = transition->getDestinationMode();
		delete transition;
		transition = nullptr;
		switchTimeout = 0;

		if (poweringOff) {
			power = false;
			poweringOff = false;
			ledManager.clear();
		}
	}

	if (power) {
		if (brightness.isChanging())
			ledManager.setBrightness(brightness.getSmoothValue());
		if (speed.isChanging())
			mode->setSpeed(speed.getSmoothValue());
		if (scale.isChanging())
			mode->setScale(scale.getSmoothValue());
		ledManager.refresh(mode);
	}
}

void Controller::switchPower() {
	transition = (power) ?
	    new CenterSlide(mode, nullptr, 1000, new CubicEase()) :
	    new CenterSlide(
	        nullptr,
	        initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	        1000, new CubicEase());
	mode = transition;

	switchTimeout = millis() + 1000;
	if (power)
		poweringOff = true;

	power = true;
}

bool Controller::poweredON() { return power; }

void Controller::nextMode() {
	modeID = (modeID + 1) % LAST_MODE;
	// TODO: Change transition to LeftSlide
	transition = new CenterSlide(
	    mode, initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	    1000, new CubicEase());
	mode = transition;
}

void Controller::previousMode() {
	modeID = (modeID - 1 + LAST_MODE) % LAST_MODE;
	// TODO: Change transition to RightSlide
	transition = new CenterSlide(
	    mode, initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	    1000, new CubicEase());
	mode = transition;
}

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