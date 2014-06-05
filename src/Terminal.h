#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"

class Terminal : public Entity {
	public:
		Terminal(b2World *world, string texture);
		~Terminal();

};