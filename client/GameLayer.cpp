#include "GameLayer.h"
#include "Map.h"
#include "MapParser.h"
#include "Player.h"
#include "ShadowLayer.h"
#include "MapLoader.h"


/*
================
CameraComponent Public
================
*/
void CameraComponent::SetPlayer(Player *player) 
{
	_player = player;
}

void CameraComponent::Update(const DeltaTime &dt)
{
	if (!_player)
		return;

	Vec2 resolution = GetGameObject()->GetApp()->GetWindow()->GetResolution();
	Vec2 offset = Vec2(resolution.x / 2.f, resolution.y / 2.f);
	Position() = Vec2(offset.x - _player->Position().x, offset.y - _player->Position().y);
}



/*
================
GameLayer Public
================
*/
GameLayer::GameLayer() 
	:	_map(NULL),
		_shadowLayer(NULL),
		_uiLayer(NULL)
{
	// Don't add child layers as children, manage them
	// manually until Z-ordering is added to Trutle.
	_shadowLayer = new ShadowLayer(this);
	_uiLayer = new Layer();

	// Load the map (this HAS to be done after the ShadowLayer has been
	// initialized).
	_map = new Map(this);
	MapParser parser("res/testmap.tmx", _map);
	if (!parser.ParseMap()) {
		Log::Error("Failed to parse map");
	} 

	MapLoader loader;
	loader.LoadMap(_map, this);

	
	// Load the player
	_player = new Player(_map, this);
	_shadowLayer->AddLightSource(GetComponent<LightSource>(_player));
	AddChild(_player);

	// Add the camera component
	AddComponent<CameraComponent>(this);
	GetComponent<CameraComponent>(this)->SetPlayer(_player);
}

GameLayer::~GameLayer() 
{
	if (_shadowLayer)
		delete _shadowLayer;
	if (_uiLayer)
		delete _uiLayer;
	if (_map)
		delete _map;
}


void GameLayer::Update(const DeltaTime& dt)
{
	Layer::Update(dt);

	_map->Update(dt);
	_player->MoveToB2Body();
}

void GameLayer::Render(Renderer* renderer)
{
	renderer->PushTransform();
	renderer->ApplyTransform(this);
	_map->RenderBackgroundLayer(renderer);
	renderer->PopTransform();
	
	Layer::Render(renderer);
	
	renderer->PushTransform();
	renderer->ApplyTransform(this);
	_shadowLayer->Render(renderer);
	_map->RenderForegroundLayer(renderer);
	_uiLayer->Render(renderer);
	renderer->PopTransform();
}


void GameLayer::AddLight(LightSource::Properties lightProps, Vec2 pos, string type)
{
	char str[256];
	sprintf(str, "(GameLayer::AddLight) "
				 "Adding light (r=%g R=%g G=%g B=%g A=%g) at (x=%g y=%g) (type=%s)",
				 lightProps.radius, lightProps.color.r, lightProps.color.g, 
				 lightProps.color.b, lightProps.color.a, pos.x, pos.y, type.c_str());
	Log::Verbose(str);

	GameObject *gob = new GameObject();
	AddComponent<LightSource>(gob);
	LightSource *light = GetComponent<LightSource>(gob);
	light->SetLightProperties(lightProps.radius, lightProps.color);
	gob->Position() = pos;

	_shadowLayer->AddLightSource(light);
	AddChild(gob);
}


ShadowLayer* GameLayer::GetShadowLayer() 
{
	return _shadowLayer;
}

