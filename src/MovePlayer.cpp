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
<<<<<<< HEAD
	Vec2 temp(0,0);
	if (in->IsKeyDown(SDLK_a))
		temp.x -= SPEED;
=======
>>>>>>> 28c3e50a868b28877471b9155ffa30211af917eb

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

