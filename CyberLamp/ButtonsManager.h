#pragma once
#include "Constants.h"

#include <GyverButton.h>
#include <queue>

class ButtonsManagerClass {
	GButton *buttons[BUTTONS_QUANTITY];
	std::queue<Instruction> pendingOps;

  public:
	ButtonsManagerClass();
	void tick();
	std::queue<Instruction> &getPendingOps();
};