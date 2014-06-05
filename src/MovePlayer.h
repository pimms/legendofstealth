#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#define PLAYER_WALK_SPEED 150
#define PLAYER_SPRINT_SPEED 200

class MovePlayer :public Component {
public:
	static float PlayerWalkSpeed();
	static float PlayerSprintSpeed();

	MovePlayer();
	void Update(const DeltaTime &dt);

private:
};

