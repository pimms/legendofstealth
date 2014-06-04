#pragma once

// Windows and Unix specific includes
#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"

class Player : public Entity {
public:
	Player(b2World *world);
private:

	
};

