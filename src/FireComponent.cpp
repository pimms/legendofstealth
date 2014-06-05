#include "FireComponent.h"
#include "Entity.h"
#include "net/Socket.h"

/*
================
FireComponent 
================
*/
FireComponent::FireComponent()
	:	_udpSocket(NULL)
{	
}

void FireComponent::SetUDPSocket(Socket *socket)
{
	_udpSocket = socket;
}


void FireComponent::Update(const DeltaTime &dt){
	const InputState *in = GetGameObject()->GetInputState();
	if (in->IsMouseKeyFresh(SDL_BUTTON_LEFT)){
		// TODO
		// Display a muzzle flash on the player
		
		// TODO
		// player->GetWorld()->RayCast(&callback, p1, p2);
		// Display a hit indicator on whatever was hit
		
		SendFirePacket();
	}
}


void FireComponent::SendFirePacket()
{
	if (!_udpSocket) {
		Log::Error("Firing a gun without giving the FireComponent an UDP Socket");
		return;
	}

	Player *player = (Player*)GetGameObject();

	PacketPlayerFire pkt;
	pkt.type = PACKET_PLAYER_FIRE;
	pkt.playerID = player->GetPlayerID();
	pkt.posX = player->Position().x;
	pkt.posY = player->Position().y;
	pkt.rotation = player->Rotation();

	_udpSocket->SendPacket(&pkt);
}


/*
================
BulletHitTester
================
*/
BulletHitTester::BulletHitTester(LocalPlayer *localp, Socket *udpSocket)
	:	_localPlayer(localp),
		_udpSocket(udpSocket)
{
}

void BulletHitTester::TestBullet(Vec2 position, float rotation)
{
	FireCallback cb(_localPlayer);
	
	b2Vec2 p1 = Tob2Vec2(_localPlayer->Position());
	b2Vec2 p2 = b2Vec2(cosf(Deg2Rad(rotation)), sinf(Deg2Rad(rotation)));
	p2 *= 1000.f;

	//_localPlayer->GetWorld()->RayCast(&cb, p1, p2);

	if (cb.DidHitTargetPlayer()) {
		SendHitPacket();
	}
}

void BulletHitTester::SendHitPacket()
{
	PacketPlayerHit pkt;
	pkt.type = PACKET_PLAYER_HIT;
	pkt.playerID = _localPlayer->GetPlayerID();

	_udpSocket->SendPacket(&pkt);
}


/*
================
FireCallback 
================
*/
FireCallback::FireCallback(Player *targetPlayer) 
	: 	_targetPlayer(targetPlayer),
		_hitPlayer(false)
{

}

float32 FireCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, 
									  const b2Vec2& normal, float32 fraction)
{
	_point = point;
	_normal = normal;
	_fraction = fraction;

	if (fixture->GetUserData()) {
		Entity *entity = (Entity*)fixture->GetUserData();
		Player *player = dynamic_cast<Player*>(entity);
		if (player) {
			if (player == _targetPlayer) {
				_hitPlayer = true;
			} else {
				// Ignore all other players
				return -1.f;
			}
		}
	}

	return fraction;
}

bool FireCallback::DidHitTargetPlayer()
{
	return _hitPlayer;
}

b2Vec2 FireCallback::GetPoint()
{
	return _point;
}
