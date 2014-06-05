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

	bool isSprinting = false;
	float speed = (isSprinting) ? PLAYER_WALK_SPEED : PLAYER_SPRINT_SPEED;

	Vec2 velocity;
	float delta = 1.f;

	if (in->IsKeyDown(SDLK_a)) {
		velocity.x = -delta;
	} else if (in->IsKeyDown(SDLK_d)) {
		velocity.x = delta;
	} else {
		velocity.x = 0.f;
	}

	if (in->IsKeyDown(SDLK_w)) { 
		velocity.y = delta;
	} else if (in->IsKeyDown(SDLK_s)) {
		velocity.y = -delta;
	} else {
		velocity.y = 0.f;
	}	

	// calculate position if len > 1
	float len = velocity.Length();
	if (len > delta) {
		velocity.x /= len;
		velocity.y /= len;
	}
	
	velocity.x *= speed;
	velocity.y *= speed;

	// update position
	Entity *entity = (Entity*)GetGameObject();
	entity->MoveB2Body(velocity);
}

