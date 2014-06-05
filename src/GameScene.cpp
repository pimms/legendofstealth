#include "GameScene.h"
#include "LightSource.h"
#include "ShadowCaster.h"
#include "Player.h"
#include "FollowMouseComponent.h"
#include "FollowGameObjectComponent.h"
#include "FireComponent.h"
#include "Terminal.h"


GameScene* GameScene::_singleton = NULL;


GameScene* GameScene::Singleton()
{
	return _singleton;
}


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
	_singleton = this;
}

GameScene::~GameScene()
{
	if (_world) {
		delete _world;
	}

	_singleton = NULL;
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
	Team team;
	unsigned playerID;

	switch (packet->type) {
		case PACKET_PLAYER_UPDATE:
			HandlePacketPlayerUpdate((PacketPlayerUpdate*)packet);
			return true;

		case PACKET_JOIN_RESPONSE:
		{
			PacketJoinResponse *pjr = (PacketJoinResponse*)packet;
			if (!pjr->response) {
				Log::Error("unable to join game session - quitting");
				GetApp()->Quit();
				return true;
			}


			team = (Team)pjr->team;
			playerID = pjr->playerID;
			CreatePlayer(team, playerID, true);
			return true;
		}

		case PACKET_PLAYER_DID_JOIN:
		{
			PacketPlayerDidJoin *p = (PacketPlayerDidJoin*)packet;
			team = (Team)p->team;
			playerID = p->playerID;
			CreatePlayer(team, playerID, false);
			return true;
		}

		case PACKET_PLAYER_FIRE:
		{	
			PacketPlayerFire *ppf = (PacketPlayerFire*)packet;
			HandlePacketPlayerFire(ppf);
			return true;
		}

		case PACKET_PLAYER_HIT:
		{	
			PacketPlayerHit *pph = (PacketPlayerHit*)packet;
			HandlePacketPlayerHit(pph);
			return true;
		}

		default:
			return false;
	}

	return false;
}


ShadowLayer* GameScene::GetShadowLayer()
{
	return _shadowLayer;
}

Layer* GameScene::GetGameLayer()
{
	return _gameLayer;
}

LocalPlayer GameScene::GetLocalPlayer() {
	return _localPlayer;
}

vector<RemotePlayer*> GameScene::GetRemotePlayers() {
	return _remotePlayers;
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

	_shadowLayer = new ShadowLayer(_gameLayer);
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
	LoadTerminal();
	
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
			player->HandleUpdatePacket(packet);
		}
	}
}

void GameScene::HandlePacketPlayerFire(const PacketPlayerFire *packet)
{
	Vec2 pos(packet->posX, packet->posY);

	BulletHitTester hit(_localPlayer, _udpSocket);
	hit.TestBullet(pos, packet->rotation);
}

void GameScene::HandlePacketPlayerHit(const PacketPlayerHit *packet)
{
	// TODO
	// Deduct one HP from the hit player
}


void GameScene::CreatePlayer(Team team, unsigned playerID, bool localPlayer)
{
	if (localPlayer) {
		_localPlayer = new LocalPlayer(_world, team, playerID, _udpSocket);
		AddComponent<FollowMouseComponent>(_localPlayer);
		_gameLayer->AddChild(_localPlayer);

		AddComponent<FollowGameObjectComponent>(_gameLayer);
		GetComponent<FollowGameObjectComponent>(_gameLayer)->SetTarget(_localPlayer, Vec2(1280.f/2.f, 720.f/2.f));
	} else {
		RemotePlayer *rp = new RemotePlayer(_world, team, playerID);
		_remotePlayers.push_back(rp);
		_gameLayer->AddChild(rp);
	}
}

void LoadTerminal() 
{
	Terminal terminal = new Terminal(_world, "res/term.png");
}
