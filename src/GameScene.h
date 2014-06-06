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


class Overlayer;
class ShadowLayer;
class RemotePlayer;
class LocalPlayer;
class Player;
class Socket;
class Terminal;
class Overlayer;
class Hackoverlay;
class RayDebugDraw;

class GameScene : public Scene {
public:
	// YOLO, gamejam etc
	static GameScene* Singleton();
	static int GetMapScalingFactor();

	GameScene(Socket *tcpSocket, Socket *udpSocket);
	~GameScene();

	void LoadContent();
	void Update(const DeltaTime &dt);

	// Returns true if the packet could be handled
	bool HandlePacket(const Packet *packet);

	LocalPlayer* GetLocalPlayer();
	vector<RemotePlayer*> GetRemotePlayers();

	ShadowLayer* GetShadowLayer();
	Layer* GetGameLayer();

	void LoadOverlay(string texture);
	void RemoveOverlay(string texture);

private:
	static GameScene *_singleton;

	b2World *_world;

	Socket *_tcpSocket;
	Socket *_udpSocket;
	
	Layer *_gameLayer;
	ShadowLayer *_shadowLayer;
	Layer *_overlayer;


	LocalPlayer *_localPlayer;
	vector<RemotePlayer*> _remotePlayers;

	Terminal *_terminal;
	Hackoverlay *_overlay;

	RayDebugDraw *_rayDraw;


	// Call the load methods in the order in which they appear plssss.
	// Call each method exactly once..
	void LoadInfrastructure();
	void CreateB2World();
	void LoadMap();
	void InitializeMap(std::string name, Vec2 scale, Vec2 pos);
	void InitalizeLight(Vec2 pos);

	void HandlePacketPlayerUpdate(const PacketPlayerUpdate *packet);
	void HandlePacketPlayerFire(const PacketPlayerFire *packet);
	void HandlePacketPlayerHit(const PacketPlayerHit *packet);

	void CreatePlayer(Team team, unsigned playerID, bool localPlayer);
	void LoadTerminal();

	void OnGameOver(unsigned playerID);
	
};
