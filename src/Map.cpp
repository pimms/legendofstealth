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
		_gameLayer(gameLayer)
{

}

Map::~Map()
{
	
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
}


/*
================
Map Private
================
*/










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
