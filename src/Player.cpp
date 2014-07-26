#include "Player.h"
#include "LightSource.h"


/*
================
MoveComponent 
================
*/
MoveComponent::MoveComponent() 
	:	_speed(PLAYER_DEFAULT_SPEED)
{

}

void MoveComponent::Update(const DeltaTime &dt) 
{
	const InputState *input = GetGameObject()->GetInputState();
	Vec2 vel(0.f, 0.f);
	
	if (input->IsKeyDown(SDLK_h)) 
		vel.x = -1.f;
	if (input->IsKeyDown(SDLK_l))
		vel.x = 1.f;
	if (input->IsKeyDown(SDLK_j))
		vel.y = -1.f;
	if (input->IsKeyDown(SDLK_k))
		vel.y = 1.f;

	float len = vel.Length();
	if (len > 1.f) {
		vel.x /= len;
		vel.y /= len;
	}

	Vec2 pos = Position();
	pos.x += vel.x * _speed * dt.dt;
	pos.y += vel.y * _speed * dt.dt;
	Position() = pos;
}





/*
================
Player Public
================
*/
Player::Player() 
{
	LoadTexture("res/player.png");

	Scale() = Vec2(0.25f, 0.25f);

	AddComponent<MoveComponent>(this);

	AddComponent<LightSource>(this);
	LightSource *ls = GetComponent<LightSource>(this);
	ls->SetLightProperties(300.f, Color(1.f, 1.f, 1.f, 0.5f));
}

Player::~Player()
{
	
}



