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

ControllerClass::ControllerClass()
    : mode(nullptr)
    , modeID(WHITE_LIGHT)
    , transition(nullptr)
    , brightness(DEFAULT_BRIGHTNESS)
    , speed(DEFAULT_SPEED)
    , scale(DEFAULT_SCALE)
    , switchTimeout(0)
    , eepromTimeout(millis() + EEPROM_FLASH_INTERVAL)
    , power(false)
    , poweringOff(false) {
	EEPROM.begin(EEPROM_SIZE);
	if (checkEEPROM())
		this->restoreState();
}

bool ControllerClass::checkEEPROM() {
	if (EEPROM.read(FIRST_BOOT_ADDRESS) == FIRST_BOOT_CHECKSUM)
		return true;

	EEPROM.write(BRIGHTNESS_ADDRESS, DEFAULT_BRIGHTNESS);
	EEPROM.write(CURRENT_MODE_ADDRESS, CURRENT_MODE_DEFAULT);

	for (uint8_t address = modeAddress(FIRST_MODE);
	     address < modeAddress(LAST_MODE); address += MODE_SIZE) {
		EEPROM.write(address, DEFAULT_SPEED);
		EEPROM.write(address + 1, DEFAULT_SCALE);
	}

	EEPROM.write(FIRST_BOOT_ADDRESS, FIRST_BOOT_CHECKSUM);
	EEPROM.commit();

	return false;
}

void ControllerClass::restoreState() {
	brightness.setValue(EEPROM.read(BRIGHTNESS_ADDRESS), 0);
	modeID = EEPROM.read(CURRENT_MODE_ADDRESS);
	speed.setValue(EEPROM.read(modeAddress(modeID)), 0);
	scale.setValue(EEPROM.read(modeAddress(modeID) + 1), 0);
}

void ControllerClass::saveState() {
	EEPROM.write(BRIGHTNESS_ADDRESS, brightness.getValue());
	EEPROM.write(CURRENT_MODE_ADDRESS, modeID);
	EEPROM.write(modeAddress(modeID), speed.getValue());
	EEPROM.write(modeAddress(modeID) + 1, scale.getValue());
	EEPROM.commit();
}

void ControllerClass::parseInstruction(Instruction instruction) {
	switch (instruction) {
		case SWITCH_POWER:
			this->switchPower();
			break;
		case NEXT_MODE:
			this->nextMode();
			break;
		case PREVIOUS_MODE:
			this->previousMode();
			break;
		case INCREASE_BRIGHTNESS:
			this->increaseBrightness();
			break;
		case DECREASE_BRIGHTNESS:
			this->decreaseBrightness();
			break;
		case INCREASE_SPEED:
			this->increaseSpeed();
			break;
		case DECREASE_SPEED:
			this->decreaseSpeed();
			break;
		case INCREASE_SCALE:
			this->increaseScale();
			break;
		case DECREASE_SCALE:
			this->decreaseScale();
			break;
	}
}

void ControllerClass::tick() {
	ButtonsManager.tick();

	std::queue<Instruction> &pendingOps = ButtonsManager.getPendingOps();
	while (!pendingOps.empty()) {
		parseInstruction(pendingOps.front());
		pendingOps.pop();
	}

	uint64_t time = millis();

	// Checking whether transition has finished
	if (transition != nullptr && time > switchTimeout) {
		mode = transition->getDestinationMode();
		delete transition;
		transition = nullptr;
		switchTimeout = 0;

		if (poweringOff) {
			power = false;
			poweringOff = false;
			LEDManager.clear();
		}
	}

	if (power) {
		if (brightness.isChanging())
			LEDManager.setBrightness(brightness.getSmoothValue());
		if (speed.isChanging())
			mode->setSpeed(speed.getSmoothValue());
		if (scale.isChanging())
			mode->setScale(scale.getSmoothValue());

		if (time > eepromTimeout) {
			this->saveState();
			eepromTimeout = time + EEPROM_FLASH_INTERVAL;
		}

		LEDManager.refresh(mode);
	}
}

void ControllerClass::switchPower() {
	if (poweringOff) {
		mode = transition->getSourceMode();
		transition = nullptr;
		switchTimeout = 0;
		poweringOff = false;
		return;
	}

	if (transition != nullptr)
		return;

	transition = (power) ?
	    new CenterSlide(mode, nullptr, ANIMATION_DURATION, new CubicEase()) :
	    new CenterSlide(
	        nullptr,
	        initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	        ANIMATION_DURATION, new CubicEase());
	mode = transition;

	if (power)
		poweringOff = true;

	power = true;
	switchTimeout = millis() + ANIMATION_DURATION;
}

bool ControllerClass::poweredON() { return power; }

void ControllerClass::nextMode() {
	if (!power)
		return;

	modeID = (modeID + 1) % LAST_MODE;
	speed.setValue(EEPROM.read(modeAddress(modeID)), 0);
	scale.setValue(EEPROM.read(modeAddress(modeID) + 1), 0);
	transition = new LeftSlide(
	    mode, initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	    ANIMATION_DURATION, new CubicEase());
	mode = transition;
	switchTimeout = millis() + ANIMATION_DURATION;
}

void ControllerClass::previousMode() {
	if (!power)
		return;

	modeID = (modeID - 1 + LAST_MODE) % LAST_MODE;
	speed.setValue(EEPROM.read(modeAddress(modeID)), 0);
	scale.setValue(EEPROM.read(modeAddress(modeID) + 1), 0);
	transition = new RightSlide(
	    mode, initMode(modeID, scale.getSmoothValue(), speed.getSmoothValue()),
	    ANIMATION_DURATION, new CubicEase());
	mode = transition;
	switchTimeout = millis() + ANIMATION_DURATION;
}

void ControllerClass::increaseBrightness() {
	if (!power)
		return;

	uint8_t temp = brightness.getValue();
	brightness.setValue(constrain(temp * 2, 1, 255));
}

void ControllerClass::decreaseBrightness() {
	if (!power)
		return;

	uint8_t temp = brightness.getValue();
	brightness.setValue(constrain(temp / 2, 1, 255));
}

void ControllerClass::increaseSpeed() {
	if (!power)
		return;

	uint8_t temp = speed.getValue();
	speed.setValue(constrain(temp + 16, 1, 255));
}

void ControllerClass::decreaseSpeed() {
	if (!power)
		return;

	uint8_t temp = speed.getValue();
	speed.setValue(constrain(temp - 16, 1, 255));
}

void ControllerClass::increaseScale() {
	if (!power)
		return;

	uint8_t temp = scale.getValue();
	scale.setValue(constrain(temp + 16, 1, 255));
}

void ControllerClass::decreaseScale() {
	if (!power)
		return;

	uint8_t temp = scale.getValue();
	scale.setValue(constrain(temp - 16, 1, 255));
}