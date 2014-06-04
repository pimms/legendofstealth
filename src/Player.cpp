#include "Player.h"
#include "PlayerUpdateComponent.h"
#include "MovePlayer.h"
#include "net/Socket.h"


/*
================
Player Public
================
*/
Player::Player(b2World *world, Team team, unsigned playerID, string texture)  
	:	Entity(world),
		_team(team),
		_playerID(playerID)
{
	LoadTexture(texture);
	Position() = Vec2(100.0f, 100.0f);

	CreateSquareBody();

	AddComponent<PlayerUpdateComponent>(this);
	_updateComponent = GetComponent<PlayerUpdateComponent>(this);
}


unsigned Player::GetPlayerID() const
{
	return _playerID;
}


Team Player::GetTeam() const
{
	return _team;
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
