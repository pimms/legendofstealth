#include "MapLoader.h"
#include "Map.h"
#include "GameLayer.h"
#include "ShadowLayer.h"
#include "ShadowCaster.h"


/*
================
MapLoader Public
================
*/
MapLoader::MapLoader() 
	:	_map(NULL),
		_gameLayer(NULL)
{

}


void MapLoader::LoadMap(Map *map, GameLayer *gameLayer) 
{
	_map = map;
	_gameLayer = gameLayer;


	Map::TileTemplate tt = _map->GetTileTemplate();

	for (int x=0; x<tt.GetWidth(); x++) {
		for (int y=0; y<tt.GetHeight(); y++) {
			unsigned flags = tt.GetFlags(x, y);
			
			if (flags & Map::TILE_SHADOW) 
				LoadShadowCaster(x, y);
		}
	}
}



/*
================
MapLoader Private 
================
*/
void MapLoader::LoadShadowCaster(int x, int y)
{
	ShadowLayer *slayer = _gameLayer->GetShadowLayer();

	GameObject *go = new GameObject();
	go->Position() = Vec2(x * MAP_TILE_SIZE, y * MAP_TILE_SIZE);
	slayer->AddChild(go);

	AddComponent<ShadowCaster>(go);
	ShadowCaster *sc = GetComponent<ShadowCaster>(go);
	slayer->AddShadowCaster(sc);

	sc->SetShadowRectangle(Rect(0.f, 0.f, MAP_TILE_SIZE, MAP_TILE_SIZE));
}
