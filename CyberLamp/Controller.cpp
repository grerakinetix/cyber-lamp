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

Controller::Controller() {
    currentModeID = WHITE_LIGHT;
    currentMode = initMode(currentModeID, 50, 50);
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
    
    if (poweredON())
        ledManager.refresh(currentMode);

}

void Controller::switchPower() {
    
}

bool Controller::poweredON() {
    return power;
}

void Controller::nextMode() {

}

void Controller::previousMode() {

}

void Controller::increaseBrightness() {
    currentBrightness = currentBrightness << 1 + 1;
}

void Controller::decreaseBrightness() {
    currentBrightness >>= 1;
}

void Controller::increaseSpeed() {

}

void Controller::decreaseSpeed() {
    
}

void Controller::increaseScale() {

}

void Controller::decreaseScale() {
    
}