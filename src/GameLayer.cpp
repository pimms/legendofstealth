#include "GameLayer.h"
#include "Map.h"
#include "MapParser.h"


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
	MapParser parser("../res/testmap.tmx");
	_map = parser.ParseMap();
	
	// Don't add child layers as children, manage them
	// manually. Until Z-ordering is added to Trutle.
	_shadowLayer = new Layer();
	_uiLayer = new Layer();
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
	
	const InputState *in = GetInputState();
	if (in->IsKeyDown(SDLK_a))
		Position().x += 250.f * dt.dt;
	if (in->IsKeyDown(SDLK_d))
		Position().x -= 250.f * dt.dt;
	if (in->IsKeyDown(SDLK_s))
		Position().y += 250.f * dt.dt;
	if (in->IsKeyDown(SDLK_w))
		Position().y -= 250.f * dt.dt;
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

