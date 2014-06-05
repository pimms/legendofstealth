#include "MovePlayer.h"
#include "Entity.h"


float MovePlayer::PlayerWalkSpeed()
{
	return PLAYER_WALK_SPEED;
}

float MovePlayer::PlayerSprintSpeed()
{
	return PLAYER_SPRINT_SPEED;
}


MovePlayer::MovePlayer() {}

void MovePlayer::Update(const DeltaTime &dt){
	const InputState *in = GetGameObject()->GetInputState();
	Vec2 temp(0,0);

	bool isSprinting = false;
	float speed = (isSprinting) ? PLAYER_WALK_SPEED : PLAYER_SPRINT_SPEED;
	
	if (in->IsKeyDown(SDLK_a))
		temp.x -= speed;
	if (in->IsKeyDown(SDLK_s))
		temp.y -= speed;
	if (in->IsKeyDown(SDLK_w))
		temp.y += speed;
	if (in->IsKeyDown(SDLK_d))
		temp.x += speed;

	// calculate position if len > 1
	float len = temp.Length();
	
	if (len > 1){
		temp.x /= len;
		temp.y /= len;
		temp.x *= dt.dt * speed;
		temp.y *= dt.dt * speed;	
	}

	// update position
	Position().x += temp.x;
	Position().y += temp.y;	
}

