#include "Map.h"
#include "MapLayer.h"

 
/*
================
Map Public
================
*/
Map::Map(GameLayer *gameLayer)
	:	_bgLayer(NULL),
		_fgLayer(NULL),
		_gameLayer(gameLayer),
		_b2World(new b2World(b2Vec2_zero))
{
	_b2World->SetDebugDraw(&_debugDraw);
	_debugDraw.AppendFlags(b2Draw::e_shapeBit);
}

Map::~Map()
{
	for (int i=0; i<_globalBodies.size(); i++) {
		_b2World->DestroyBody(_globalBodies[i]);
	}

	_globalBodies.clear();
	delete _b2World;
}


void Map::SetBackgroundLayer(MapLayer* layer)
{
	_bgLayer = layer;
	AddChild(_bgLayer);
}

void Map::SetForegroundLayer(MapLayer* layer)
{
	_fgLayer = layer;
	AddChild(_fgLayer);
}


void Map::SetTileTemplate(Map::TileTemplate tt) 
{
	_tileTemplate = tt;
}

Map::TileTemplate Map::GetTileTemplate() const
{
	return _tileTemplate;
}


b2World* Map::GetB2World() const
{
	return _b2World;
}

GameLayer* Map::GetGameLayer() const
{
	return _gameLayer;
}


void Map::Update(const DeltaTime &dt)
{
	GameObject::Update(dt);

	_b2World->Step(1.f / 60.f, 10, 10);
}


void Map::Render(Renderer* renderer)
{
	// Do nothing
}

void Map::RenderBackgroundLayer(Renderer* renderer)
{
	if (_bgLayer)
		_bgLayer->Render(renderer);
}

void Map::RenderForegroundLayer(Renderer* renderer)
{
	if (_fgLayer)
		_fgLayer->Render(renderer);

	_b2World->DrawDebugData();
}


/*
================
Map Private
================
*/
void Map::AddGlobalBody(b2Body *body) 
{
	_globalBodies.push_back(body);
}




/*
================
Map::TileTemplate 
================
*/
Map::TileTemplate::TileTemplate() 
{ 
	_w = 0;
	_h = 0;
}

Map::TileTemplate::TileTemplate(int mapWidth, int mapHeight) 
{
	_w = mapWidth;
	_h = mapHeight;

	_tiles.resize(mapWidth * mapHeight, 0);
}

void Map::TileTemplate::AddFlag(int x, int y, unsigned flag) 
{
	_tiles[y * _w + x] |= flag;
}


int Map::TileTemplate::GetWidth() const
{
	return _w;
}

int Map::TileTemplate::GetHeight() const
{
	return _h;
}

unsigned Map::TileTemplate::GetFlags(int x, int y) const
{
	return _tiles[y * _w + x];
}
