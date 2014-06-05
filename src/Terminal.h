#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"

#define HACKTIME 100;

class Terminal : public Entity {
	public:
		Terminal(b2World *world, string texture);
		~Terminal();

};