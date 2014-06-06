#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include <Box2D/Box2D.h>
#include "Player.h"

#define FIRE_LENGTH 10;

class Socket;
class FireCallback;

/* FireComponent
 * When locally firing the gun, the client sends a Fire-packet to the server.
 * The server broadcasts this message. Others players check if the bullet
 * hit themselves (IGNORING ALL OTHER PLAYERS), and reports back to the server
 * if they were hit.
 */
class FireComponent : public Component {
public:
	static FireCallback TestFire(LocalPlayer *local, Vec2 pos, float rot);

	FireComponent();
	void SetUDPSocket(Socket *udpSocket);
	
	void Update(const DeltaTime &dt);

private:
	Socket *_udpSocket;

	void SendFirePacket();
};


/* Bullet Hit Tester
 * Tests whether or not a bullet hit the local player. Sends a PacketPlayerHit
 * message to the server if it was hit.
 */
class BulletHitTester {
public:
	BulletHitTester(LocalPlayer *player, Socket *udpSocket);

	void TestBullet(Vec2 position, float rotation);

	void GetLine(b2Vec2 &p1, b2Vec2 &p2);

private:
	LocalPlayer *_localPlayer;
	Socket *_udpSocket;

	b2Vec2 _p1;
	b2Vec2 _p2;

	void SendHitPacket();
};


class FireCallback : public b2RayCastCallback {
public:
	FireCallback(Player *targetPlayer);

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
						  const b2Vec2& normal, float32 fraction);

	bool DidHitTargetPlayer();
	b2Vec2 GetPoint();
	
private:
	b2Vec2		_point;
	b2Vec2		_normal;
	float32		_fraction;
	bool 		_hitPlayer;
	Player 		*_targetPlayer;
};
