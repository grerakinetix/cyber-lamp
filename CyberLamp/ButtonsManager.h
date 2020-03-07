#pragma once
#include "Constants.h"

#include <GyverButton.h>
#include <queue>

class ButtonsManager {
	GButton *buttons[BUTTONS_QUANTITY];
	std::queue<Instruction> pendingOps;

  public:
	ButtonsManager();
	void tick();
	std::queue<Instruction> &getPendingOps();
};