#include "Player.h"
#include "PlayerUpdateComponent.h"
#include "MovePlayer.h"
#include "net/Socket.h"
#include "LightSource.h"
#include "GameScene.h"


/*
================
Player Public
================
*/
Player::Player(b2World *world, Team team, unsigned playerID, string texture)  
	:	Entity(world),
		_team(team),
		_playerID(playerID),
		_lightSource(NULL)
{
	LoadTexture(texture);
	Position() = Vec2(100.0f, 100.0f);

	CreateSquareBody();

	AddComponent<PlayerUpdateComponent>(this);
	_updateComponent = GetComponent<PlayerUpdateComponent>(this);

	if (team == TEAM_MERC) {
		ConeLightSource *light = new ConeLightSource();
		light->SetLightDistance(350.f);
		AddChild(light);
		GameScene::Singleton()->GetShadowLayer()->AddLightSource(light);
	}
}


unsigned Player::GetPlayerID() const
{
	return _playerID;
}


Team Player::GetTeam() const
{
	return _team;
}


void Player::Update(const DeltaTime &dt)
{
	Entity::Update(dt);

	if (_lightSource) {
		_lightSource->Rotation() = Rotation();
	}
}


/*
================
LocalPlayer Public
================
*/
LocalPlayer::LocalPlayer(b2World *world, Team team, unsigned playerID, Socket *udpSocket)
	:	Player(world, team, playerID),
		_udpSocket(udpSocket)
{
	AddComponent<MovePlayer>(this);
	_updateComponent->SetUDPSocket(_udpSocket);
}


void LocalPlayer::Update(const DeltaTime &dt)
{
	Player::Update(dt);
	_updateComponent->SendUpdatePacket();
}


/*
================
RemotePlayer Public
================
*/
RemotePlayer::RemotePlayer(b2World *world, Team team, unsigned playerID)
	:	Player(world, team, playerID)
{
	
}

void RemotePlayer::HandleUpdatePacket(const PacketPlayerUpdate *packet)
{
	_updateComponent->HandleUpdatePacket(packet);
}
