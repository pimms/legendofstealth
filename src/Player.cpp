#include "Player.h"
#include "MovePlayer.h"


Player::Player(b2World *world)  
	:	Entity(world)
{
	LoadTexture("res/box.png");
	Position() = Vec2(100.0f, 200.0f);
	CreateSquareBody();
	AddComponent<MovePlayer>(this);
}
