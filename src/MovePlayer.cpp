#include "MovePlayer.h"

MovePlayer::MovePlayer() {}


void MovePlayer::Update(const DeltaTime &dt){
	const InputState *in = GetGameObject()->GetInputState();
	Vec2 temp(0,0);
	
	if (in->IsKeyDown(SDLK_a))
		temp.x -= SPEED;

	if (in->IsKeyDown(SDLK_s))
		temp.y -= SPEED;

	if (in->IsKeyDown(SDLK_w))
		temp.y += SPEED;
	if (in->IsKeyDown(SDLK_d))
		temp.x += SPEED;

	// calculate position if len > 1
	float len = temp.Length();
	
	
	if (len > 1){
		temp.x /= len;
		temp.y /= len;
		temp.x *= dt.dt * SPEED;
		temp.y *= dt.dt * SPEED;	
	}

	// update position
	Position().x += temp.x;
	Position().y += temp.y;	
}

