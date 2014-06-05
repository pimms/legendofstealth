#pragma once

#ifdef _WIN32
	#include <Trutle.h>
	#include <Box2D/Box2D.h>
#else
	#include <trutle/Trutle.h>
    #include <Box2D/Box2D.h>

#endif

#include <Box2D/Box2D.h>
#include "net/Packet.h"
#include "ShadowLayer.h"


class ShadowLayer;
class RemotePlayer;
class LocalPlayer;
class Player;
class Socket;

class GameScene : public Scene {
public:
	// YOLO, gamejam etc
	static GameScene* Singleton();

	GameScene(Socket *tcpSocket, Socket *udpSocket);
	~GameScene();

	void LoadContent();
	void Update(const DeltaTime &dt);

	// Returns true if the packet could be handled
	bool HandlePacket(const Packet *packet);

	ShadowLayer* GetShadowLayer();
	Layer* GetGameLayer();

private:
	static GameScene *_singleton;

	b2World *_world;

	Socket *_tcpSocket;
	Socket *_udpSocket;
	
	Layer *_gameLayer;
	ShadowLayer *_shadowLayer;

	LocalPlayer *_localPlayer;
	vector<RemotePlayer*> _remotePlayers;

	// Call the load methods in the order in which they appear plssss.
	// Call each method exactly once..
	void LoadInfrastructure();
	void CreateB2World();
	void LoadMap();

	void HandlePacketPlayerUpdate(const PacketPlayerUpdate *packet);

	void CreatePlayer(Team team, unsigned playerID, bool localPlayer);

};
