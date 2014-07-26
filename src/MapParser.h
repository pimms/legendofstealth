#pragma once 

#include <trutle/Trutle.h>
#include <TmxParser/Tmx.h>
#include "LightSource.h"

class Map;
class MapLayer;

/* MapParser
 * Produces a Map instance given a TMX file. The TMX files must
 * conform to a few standards - see the tech doc for further
 * details.
 */
class MapParser {
public:
	MapParser(string mapFile);
	~MapParser();
	
	Map* ParseMap();

private:
	string _mapFile;
	Tmx::Map _map;
	Tmx::Tileset *_mapset;
	Tmx::Tileset *_objset;
	
	// Parse the Background or Foreground layer
	MapLayer* ParseDrawableLayer(Tmx::Layer *layer);
	Rect GetTextureClip(Tmx::Tileset *set, int gid);
	Rect GetWorldClip(Tmx::Tileset *set, int x, int y);

	// Parse the light layer
	vector<LightSource::Properties> ParseLightLayer(Tmx::Layer *layer);
};
