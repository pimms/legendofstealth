#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

<<<<<<< HEAD
#define SPEED 30;
=======
#define PLAYER_WALK_SPEED 150
#define PLAYER_SPRINT_SPEED 200
>>>>>>> 28c3e50a868b28877471b9155ffa30211af917eb

class MovePlayer :public Component {
public:
	static float PlayerWalkSpeed();
	static float PlayerSprintSpeed();

	MovePlayer();
	void Update(const DeltaTime &dt);

private:
};

