#include "Player.h"


Player::Player(b2World *world)  
	:	Entity(world)
{
	LoadTexture("res/box.png");
	Position() = Vec2(100.0f, 100.0f);

	CreateSquareBody();
}
