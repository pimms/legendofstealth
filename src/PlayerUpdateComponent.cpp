#include "PlayerUpdateComponent.h"
#include "net/Socket.h"
#include "net/Packet.h"
#include "Player.h"


/*
================
PlayerUpdateComponent Public 
================
*/
PlayerUpdateComponent::~PlayerUpdateComponent()
{
	
}

void PlayerUpdateComponent::Update(const DeltaTime &dt)
{

}


void PlayerUpdateComponent::SendUpdatePacket()
{
	PacketPlayerUpdate packet;

	packet.type = PACKET_PLAYER_UPDATE;
	packet.posX = Position().x;
	packet.posY = Position().y;
	packet.rotation = Rotation();
	packet.counter = 0;
	packet.playerID = ((Player*)GetGameObject())->GetPlayerID();

	if (_udpSocket) {
		_udpSocket->SendPacket(&packet);
	}
}

void PlayerUpdateComponent::HandleUpdatePacket(const PacketPlayerUpdate *packet)
{
	Position().x = packet->posX;
	Position().y = packet->posY;
	Rotation() = packet->rotation;
}


/*
================
PlayerUpdateComponent private
================
*/
