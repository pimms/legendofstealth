#include "GameScene.h"
#include "ShadowLayer.h"
#include "LightSource.h"
#include "ShadowCaster.h"
#include "Player.h"

/*
================
GameScene Public
================
 */
GameScene::GameScene(Socket *tcp, Socket *udp)
	:	_localPlayer(NULL),
		_gameLayer(NULL),
		_shadowLayer(NULL),
		_world(NULL),
		_tcpSocket(tcp),
		_udpSocket(udp)
{

}

GameScene::~GameScene()
{
	if (_world) {
		delete _world;
	}
}


void GameScene::LoadContent()
{
	Scene::LoadContent();

	LoadInfrastructure();
	CreateB2World();
	LoadMap();
}

void GameScene::Update(const DeltaTime &dt)
{
	Scene::Update(dt);

	_world->Step(dt.dt, 10, 10);
}


bool GameScene::HandlePacket(const Packet *packet)
{
	switch (packet->type) {
		case PACKET_PLAYER_UPDATE:
			HandlePacketPlayerUpdate((PacketPlayerUpdate*)packet);
			return true;

		case PACKET_JOIN_RESPONSE:
			CreatePlayer((PacketJoinResponse*)packet, true);
			return true;

		case PACKET_PLAYER_DID_JOIN:
			CreatePlayer((PacketJoinResponse*)packet, false);
			return true;

		default:
			return false;
	}
}


/*
================
GameScene private
================
*/
void GameScene::LoadInfrastructure()
{
	_gameLayer = new Layer();
	AddLayer(_gameLayer);

	_shadowLayer = new ShadowLayer();
	AddLayer(_shadowLayer);
}

void GameScene::CreateB2World()
{
	_world = new b2World(b2Vec2(0.f, 0.f));	
}

void GameScene::LoadMap()
{
	GameObject *background = new GameObject;
	background->LoadTexture("res/bg.png");
	background->Position() = Vec2(320.f, 240.f);
	_gameLayer->AddChild(background);
	
	// Create a couple of lights
	for (int i=0; i<4; i++) {
		LightSource *light = new LightSource;
		light->SetLightDistance(150.f);
		light->LoadTexture("res/light.png");
		light->Position() = Vec2(100.f + 120*i, 200.f);
		light->Pivot() = Vec2(0.5f, 0.5f);
		_gameLayer->AddChild(light);
		_shadowLayer->AddLightSource(light);
		
		if (rand() % 2 == 0) {
			Color c(1.f, 0.f, 0.f, 1.f);
			light->SetColor(c);
		} else {
			Color c(0.f, 0.f, 1.f, 1.f);
			light->SetColor(c);
		}
	}

	// Create a couple of boxes
	for (int i=0; i<5; i++) {
		ShadowCaster *box = new ShadowCaster;
		box->LoadTexture("res/box.png");
		box->Position() = Vec2(100.f + 70*i, 240.f);
		_gameLayer->AddChild(box);
		_shadowLayer->AddShadowCaster(box);
	}

}


void GameScene::HandlePacketPlayerUpdate(const PacketPlayerUpdate *packet)
{
	for (int i=0; i<_remotePlayers.size(); i++) {
		RemotePlayer *player = _remotePlayers[i];
		if (player->GetPlayerID() == packet->playerID) {
			
		}
	}
}


void GameScene::CreatePlayer(const PacketJoinResponse *packet, bool localPlayer)
{
	if (!packet->response) {
		Log::Error("Unable to join game session - quitting");
		GetApp()->Quit();
		return;
	}

	if (localPlayer) {
		_localPlayer = new LocalPlayer(_world, (Team)packet->team, packet->playerID, _udpSocket);
		_gameLayer->AddChild(_localPlayer);
	} else {
		RemotePlayer *rp = new RemotePlayer(_world, (Team)packet->team, packet->playerID);
		_remotePlayers.push_back(rp);
	}
}
