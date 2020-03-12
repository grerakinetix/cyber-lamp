#include "ButtonsManager.h"

ButtonsManagerClass::ButtonsManagerClass() {
	uint8_t pins[BUTTONS_QUANTITY] = BUTTON_PINS;
	for (int i = 0; i < BUTTONS_QUANTITY; ++i) {
		buttons[i] = new GButton(pins[i], LOW_PULL, NORM_OPEN);
		buttons[i]->setStepTimeout(250);
		buttons[i]->setClickTimeout(500);
	}
}

void ButtonsManagerClass::tick() {
	for (int i = 0; i < BUTTONS_QUANTITY; ++i)
		buttons[i]->tick();

	if (buttons[CENTER]->isSingle())
		pendingOps.push(SWITCH_POWER);

#if (BUTTONS_QUANTITY < 5)
	if (buttons[CENTER]->isDouble())
		pendingOps.push(NEXT_MODE);
	if (buttons[CENTER]->isTriple())
		pendingOps.push(PREVIOUS_MODE);
#else
	if (buttons[RIGHT]->isClick())
		pendingOps.push(NEXT_MODE);
	if (buttons[LEFT]->isClick())
		pendingOps.push(PREVIOUS_MODE);
	if (buttons[RIGHT]->isStep(0))
		pendingOps.push(INCREASE_SCALE);
	if (buttons[LEFT]->isStep(0))
		pendingOps.push(DECREASE_SCALE);
#endif

#if (BUTTONS_QUANTITY < 3)
	if (buttons[CENTER]->isStep(1))
		pendingOps.push(INCREASE_BRIGHTNESS);
	if (buttons[CENTER]->isStep(2))
		pendingOps.push(DECREASE_BRIGHTNESS);
#else
	if (buttons[UP]->isStep(0) || buttons[UP]->isClick())
		pendingOps.push(INCREASE_BRIGHTNESS);
	if (buttons[DOWN]->isStep(0) || buttons[DOWN]->isClick())
		pendingOps.push(DECREASE_BRIGHTNESS);
	if (buttons[UP]->isStep(1))
		pendingOps.push(INCREASE_SPEED);
	if (buttons[DOWN]->isStep(1))
		pendingOps.push(DECREASE_SPEED);
#endif
}

std::queue<Instruction> &ButtonsManagerClass::getPendingOps() {
	return pendingOps;
}