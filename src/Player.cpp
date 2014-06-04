#include "Player.h"
#include "PlayerUpdateComponent.h"
#include "MovePlayer.h"


Player::Player(b2World *world, bool isLocal)  
	:	Entity(world),
		_local(isLocal)
{
	LoadTexture("res/box.png");
	Position() = Vec2(100.0f, 100.0f);

	CreateSquareBody();

	AddComponent<MovePlayer>(this);
	AddComponent<PlayerUpdateComponent>(this);
	_updateComponent = GetComponent<PlayerUpdateComponent>(this);
}


void Player::SetPlayerID(unsigned id)
{
	_playerID = id;
}

unsigned Player::GetPlayerID() const
{
	return _playerID;
}


void Player::Update(const DeltaTime &dt)
{
	Entity::Update(dt);

	if (_local) {
		_updateComponent->SendUpdatePacket();
	}
}
