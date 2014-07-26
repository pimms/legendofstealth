#include "GameLayer.h"
#include "Map.h"
#include "MapParser.h"
#include "Player.h"
#include "ShadowLayer.h"
#include "MapLoader.h"


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

	_player = new Player();
	_shadowLayer->AddLightSource(GetComponent<LightSource>(_player));
	AddChild(_player);


	// Load the map (this HAS to be done after the ShadowLayer has been
	// initialized).
	_map = new Map(this);
	MapParser parser("res/testmap.tmx", _map);
	if (!parser.ParseMap()) {
		Log::Error("Failed to parse map");
	} 

	MapLoader loader;
	loader.LoadMap(_map, this);
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


ShadowLayer* GameLayer::GetShadowLayer() 
{
	return _shadowLayer;
}
