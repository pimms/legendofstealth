#include "Player.h"
#include "LightSource.h"
#include "Map.h"


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


	if (_body) {
		b2Vec2 b2vel(vel.x, vel.y);
		b2vel *= _speed * dt.dt;
		_body->SetLinearVelocity(b2vel);
	} else {
		// Manual movement
		Vec2 pos = Position();
		pos.x += vel.x * _speed * B2_PTM * dt.dt;
		pos.y += vel.y * _speed * B2_PTM * dt.dt;
		Position() = pos;
	}

	printf("%g %g\n", Position().x, Position().y);
}


void MoveComponent::SetBody(b2Body *body)
{
	_body = body;
}



/*
================
Player Public
================
*/
Player::Player(Map *map, GameLayer *gameLayer) 
	:	_body(NULL)
{
	LoadTexture("res/player.png");

	Scale() = Vec2(0.25f, 0.25f);

	AddComponent<MoveComponent>(this);

	AddComponent<LightSource>(this);
	LightSource *ls = GetComponent<LightSource>(this);
	ls->SetLightProperties(300.f, Color(1.f, 1.f, 1.f, 0.5f));

	CreatePhysics(map->GetB2World());
	GetComponent<MoveComponent>(this)->SetBody(_body);
}

Player::~Player()
{
	if (_body) {
		_body->GetWorld()->DestroyBody(_body);
		_body = NULL;
	}
}


void Player::MoveToB2Body()
{
	Position() = ToVec2(_body->GetPosition());
}


/*
================
Player Private 
================
*/
void Player::CreatePhysics(b2World *world)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.allowSleep = false;
	bd.userData = this;

	b2CircleShape shape;
	shape.m_radius = MAP_TILE_SIZE / B2_PTM / 2.f * 0.8f; 
	
	b2FixtureDef fd;
	fd.filter.categoryBits = ColliderGroups::PLAYER;
	fd.filter.maskBits = 	ColliderGroups::WALL
						|	ColliderGroups::VENTILATION
						|	ColliderGroups::PLAYER;
	fd.density = 1.5f;
	fd.friction = 1.f;
	fd.userData = this;
	fd.shape = &shape;

	_body = world->CreateBody(&bd);
	_body->CreateFixture(&fd);
}

