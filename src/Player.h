#pragma once

// Windows and Unix specific includes
#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"
#include "PlayerUpdateComponent.h"
#include "MuzzleFlash.h"

class Socket;
class LightSource;

class Player : public Entity {
public:
	Player(b2World *world, Team team, unsigned playerID, string texture="res/box.png");

	unsigned GetPlayerID() const;
	Team GetTeam() const;

	void Update(const DeltaTime &dt);

	void Respawn();

	void DisplayMuzzleFlash();
	void DisplayBloodSplat();

protected:
	PlayerUpdateComponent *_updateComponent;

private:
	unsigned _playerID;
	Team _team;
	LightSource *_lightSource;

	MuzzleFlash *_flash;
	BloodSplat *_splat;

	Vec2 _spawnPosition;
};


class LocalPlayer : public Player {
public:
	LocalPlayer(b2World *world, Team team, unsigned playerID, Socket *udpSocket);

	void Update(const DeltaTime &dt);

private:
	Socket *_udpSocket;
};


class RemotePlayer : public Player {
public:
	RemotePlayer(b2World *world, Team team, unsigned playerID);

	void Update(const DeltaTime &dt);
	void HandleUpdatePacket(const PacketPlayerUpdate *packet);
};
