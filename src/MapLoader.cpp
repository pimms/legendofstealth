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

			if (flags & Map::TILE_WALL) {
				bool vent = (flags & Map::TILE_VENT);
				LoadWall(x, y, vent);
			}
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

void MapLoader::LoadWall(int x, int y, bool isVentilation) 
{
	b2World *world = _map->GetB2World();

	float metersWide = (float)B2METER_PER_TILE / 2.f;
	unsigned category = (isVentilation) 
		? ColliderGroups::WALL 
		: ColliderGroups::VENTILATION;


	b2PolygonShape shape;
	shape.SetAsBox(metersWide, metersWide);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.filter.categoryBits = category;
	fd.filter.maskBits = ColliderGroups::PLAYER;
	
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.fixedRotation = true;
	bd.position = b2Vec2(
		(MAP_TILE_SIZE * ((float)x + 0.5f)) / B2_PTM, 
		(MAP_TILE_SIZE * ((float)y + 0.5f)) / B2_PTM
	);
	
	b2Body *body = world->CreateBody(&bd);
	body->CreateFixture(&fd);

	_map->AddGlobalBody(body);
}

