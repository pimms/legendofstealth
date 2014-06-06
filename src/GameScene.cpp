#include "GameScene.h"
#include "LightSource.h"
#include "ShadowCaster.h"
#include "Player.h"
#include "FollowMouseComponent.h"
#include "Terminal.h"
#include "FollowGameObjectComponent.h"
#include "FireComponent.h"
#include "Terminal.h"
#include "Walls.h"
#include "Hackoverlay.h"
#include "RayDebugDraw.h"

//
GameScene* GameScene::_singleton = NULL;


GameScene* GameScene::Singleton()
{
	return _singleton;
}

int GameScene::GetMapScalingFactor()
{
	return 30;
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


	_rayDraw = new RayDebugDraw();
	_gameLayer->AddChild(_rayDraw);
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

	if (_bullethit) {
		_localPlayer->DeductHP();
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

void GameScene::LoadOverlay(string texture) 
{
	_overlay = new Hackoverlay(_world, texture, Vec2(200.f, 100.f));
	_overlayer->AddChild(_overlay);
}

void GameScene::RemoveOverlay(string texture) {
	_overlayer->RemoveChild(_overlay);
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
	_shadowLayer->SetShadowColor(Color(0.f, 0.f, 0.f, 1.f));
	AddLayer(_shadowLayer);

	_overlayer = new Layer();
	AddLayer(_overlayer);	
}

void GameScene::CreateB2World()
{
	_world = new b2World(b2Vec2(0.f, 0.f));	
}

void GameScene::LoadMap()
{
	GameObject *bg = new GameObject();
	bg->LoadTexture("res/background.png");
	bg->Pivot() = Vec2(0.f, 0.f);
	_gameLayer->AddChild(bg);
	
	// Walls (1,7)
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(17, 63));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(17, 53));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(52, 63));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(52, 53));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(11, 61));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(24, 39));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(16, 31));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(27, 18));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(24, 5));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(24, 5));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(45, 39));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(53, 31));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(42, 18));
	InitializeMap("res/wall2D.png", Vec2(1, 7), Vec2(45, 5));

	// Walls(7,1)
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(0, 53));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(10, 53));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(53, 53));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(63, 53));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(17, 38));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(46, 38));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(4, 9));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(59, 9));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(17, 4));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(46, 4));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(28, 12));
	InitializeMap("res/wall2D.png", Vec2(7, 1), Vec2(35, 12));

	// Walls (2,11)
	InitializeMap("res/wall2D.png", Vec2(2, 11), Vec2(34, 58));
	InitializeMap("res/wall2D.png", Vec2(2, 11), Vec2(34, 41));
	InitializeMap("res/wall2D.png", Vec2(2, 11), Vec2(34, 26));

	// Walls (11,2)
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(6, 45));
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(53, 45));
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(0, 29));
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(59, 29));
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(9, 19));
	InitializeMap("res/wall2D.png", Vec2(11, 2), Vec2(50, 19));

	// Walls (1,3)
	InitializeMap("res/wall2D.png", Vec2(1, 3), Vec2(59, 56));

	// Walls (3,1)
	InitializeMap("res/wall2D.png", Vec2(3, 1), Vec2(56, 59));
	InitializeMap("res/wall2D.png", Vec2(3, 1), Vec2(3, 60));

	// Walls around the map (1,70)
	InitializeMap("res/wall2D.png", Vec2(1, 70), Vec2(0, 0));
	InitializeMap("res/wall2D.png", Vec2(1, 70), Vec2(69, 0));


	// Walls around the map(68,1)
	InitializeMap("res/wall2D.png", Vec2(68, 1), Vec2(1, 0));
	InitializeMap("res/wall2D.png", Vec2(68, 1), Vec2(1, 69));
	
	// Create light
	InitalizeLight(Vec2(57, 60));
	InitalizeLight(Vec2(65, 54));
	InitalizeLight(Vec2(64, 45));
	InitalizeLight(Vec2(64, 41));
	InitalizeLight(Vec2(64, 15));
	InitalizeLight(Vec2(64, 5));
	InitalizeLight(Vec2(50, 10));

	InitalizeLight(Vec2(45, 4));
	InitalizeLight(Vec2(24, 4));
	InitalizeLight(Vec2(5, 5));
	InitalizeLight(Vec2(20, 10));
	InitalizeLight(Vec2(5, 16));
	InitalizeLight(Vec2(10, 25));

	InitalizeLight(Vec2(6, 31));
	InitalizeLight(Vec2(16, 38));
	InitalizeLight(Vec2(5, 45));
	InitalizeLight(Vec2(11, 60));
	InitalizeLight(Vec2(22, 52));
	InitalizeLight(Vec2(24, 38));

	InitalizeLight(Vec2(33, 34));
	InitalizeLight(Vec2(42, 15));
	InitalizeLight(Vec2(27, 18));
	InitalizeLight(Vec2(42, 25));
	InitalizeLight(Vec2(27, 25));
	InitalizeLight(Vec2(25, 38));

	InitalizeLight(Vec2(53, 38));
	InitalizeLight(Vec2(21, 64));
	InitalizeLight(Vec2(48, 64));
	InitalizeLight(Vec2(40, 60));
	InitalizeLight(Vec2(29, 60));
	InitalizeLight(Vec2(46, 52));
	InitalizeLight(Vec2(22, 52));

	// Create BOXES
	InitializeMap("res/crate.png",Vec2(1,1) , Vec2(10,66));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(16, 52));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(27, 65));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(37, 68));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(36, 59));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(36, 50));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(56, 65));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(68, 48));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(66, 45));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(40, 30));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(46, 28));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(46, 29));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(47, 28));

	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(67, 24));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(68, 25));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(68, 20));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(68, 1));

	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(43, 4));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(27, 8));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(1, 1));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(1, 2));

	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(2, 1));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(10, 8));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(15, 18));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(3, 22));

	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(22, 31));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(22, 32));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(23, 31));
	InitializeMap("res/crate.png", Vec2(1, 1), Vec2(23, 32));

	LoadTerminal();
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

	b2Vec2 p1, p2;
	hit.GetLine(p1, p2);
	_rayDraw->SetRay(p1, p2);

	Player *p = NULL;
	for (int i=0; i<_remotePlayers.size(); i++) {
		if (_remotePlayers[i]->GetPlayerID() == packet->playerID) {
			p = _remotePlayers[i];
		}
	}

	if (p) {
		Vec2 pos = p->Position();
		printf("BULLET OFFSET: %g %g\n", pos.x - ToVec2(p1).x, pos.y - ToVec2(p1).y);
		printf("PACKET OFFSET: %g %g\n", pos.x - packet->posX, pos.y - packet->posY);
	}

}

void GameScene::HandlePacketPlayerHit(const PacketPlayerHit *packet)
{
	_bullethit = true;
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

void GameScene::LoadTerminal() 
{
	_terminal = new Terminal(_world, "res/term.png", Vec2(34.f, 6.f));
	_gameLayer->AddChild(_terminal);
}


void GameScene::InitializeMap(std::string name, Vec2 scale, Vec2 pos)
{
	float s = GetMapScalingFactor();
	scale.x *= s/100.f;
	scale.y *= s/100.f;

	pos.x *= GetMapScalingFactor();
	pos.y *= GetMapScalingFactor();

	Walls* go = new Walls(_world, name, scale, pos);
	go->SetPosition(go->Position());

	_gameLayer->AddChild(go);
	_shadowLayer->AddShadowCaster(go);
}

void GameScene::InitalizeLight(Vec2 pos)

{
	LightSource* temp = new LightSource(true);
	temp->SetLightDistance(7.5f * GetMapScalingFactor());
	pos.x *= GetMapScalingFactor();
	pos.y *= GetMapScalingFactor();
	temp->Position() = pos;
	temp->Pivot() = Vec2(0, 0);
	_gameLayer->AddChild(temp);
	_shadowLayer->AddLightSource(temp);

	if (rand() % 2 == 0) {
		Color c(1.f, 0.f, 0.f, 1.f);
		temp->SetColor(c);
	}
	else {
		Color c(0.f, 0.f, 1.f, 1.f);
		temp->SetColor(c);
	}
}

