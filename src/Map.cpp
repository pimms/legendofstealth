#include "Map.h"
#include "MapLayer.h"

 
/*
================
Map Public
================
*/
Map::Map()
	:	_bgLayer(NULL),
		_fgLayer(NULL)
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