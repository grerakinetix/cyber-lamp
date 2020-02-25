#pragma once
#include <GyverButton.h>
#include <queue>

#include "Constants.h"

class ButtonsManager {
	GButton *buttons[BUTTONS_QUANTITY];
	std::queue<Instruction> pendingOps;

  public:
	ButtonsManager();
	void tick();
	std::queue<Instruction> &getPendingOps();
};