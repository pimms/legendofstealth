#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include <Box2D\Box2D.h>
#include "RayCastUtility.h"
#include "Player.h"

#define FIRE_LENGTH 10;


class FireComponent : public Component {
public:
	FireComponent();
	void Update(const DeltaTime &dt);
	b2Vec2 calculateDirection();

	

};