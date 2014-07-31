#pragma once
#include <trutle/Trutle.h>


class Map;
class GameLayer;


class MapLoader {
public:
	MapLoader();
	void LoadMap(Map *map, GameLayer *gamelayer);

private:
	Map *_map;
	GameLayer *_gameLayer;

	void LoadShadowCaster(int x, int y);
	
	void LoadWall(int x, int y, bool isVentilation);
};
