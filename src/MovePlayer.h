#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#define SPEED 100;

class MovePlayer :public Component {

public:
	MovePlayer();
	void Update(const DeltaTime &dt);
};

