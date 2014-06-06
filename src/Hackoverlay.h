#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"

class Hackoverlay : public Entity {
	public:
		Hackoverlay(b2World *world, string texture, Vec2 pos);
		~Hackoverlay();
};