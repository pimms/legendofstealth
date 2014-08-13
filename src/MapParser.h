#pragma once 

#include <trutle/Trutle.h>
#include <TmxParser/Tmx.h>
#include "LightSource.h"
#include "Map.h"

class MapLayer;

/* MapParser
 * Produces a Map instance given a TMX file. The TMX files must
 * conform to a few standards - see the tech doc for further
 * details.
 */
class MapParser {
public:
	MapParser(string mapFile, Map *map);
	~MapParser();
	
	bool ParseMap();

private:
	enum ObjectsetTileID {
		ID_TERM_A 		= 0,
		ID_TERM_B 		= 1,
		ID_TERM_C 		= 2,
		ID_WALL 		= 5,
		ID_VENT 		= 6,
	};

	string _mapFile;
	Map *_map;
	Tmx::Map _tmxMap;
	Tmx::Tileset *_mapset;
	Tmx::Tileset *_objset;
	
	// Parse the Background or Foreground layer
	MapLayer* ParseDrawableLayer(Tmx::Layer *layer);
	Rect GetTextureClip(Tmx::Tileset *set, int gid);
	Rect GetWorldClip(Tmx::Tileset *set, int x, int y);

	// Parse the walls-layer
	void ParseWallLayer(Tmx::Layer *layer, Map::TileTemplate &tileTemplate);

	// Parse the light-layer
	void ParseLightLayer(const Tmx::ObjectGroup *objGroup);

	// TMX maps use a top-left origo while we use a bottom-left.
	// Inverse a Y-coordinate based on the height of the map.
	int FlipY(int mapY);
	int FlipPixelY(int mapY);
};
