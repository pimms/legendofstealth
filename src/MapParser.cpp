#include "MapParser.h"
#include "MapLayer.h"


/*
================
MapParser Public
================
*/
MapParser::MapParser(string mapFile, Map *map)
	:	_mapFile(mapFile),
		_mapset(NULL),
		_objset(NULL),
		_map(map)
{
	// Locate mapset and objectset
	_tmxMap.ParseFile(mapFile);
	
	if (_tmxMap.HasError()) {
		string msg = "Failed to parse map: ";
		msg += _tmxMap.GetErrorText();
		Log::Error(msg);
		throw std::runtime_error(msg);
	}
	
	vector<Tmx::Tileset*> tsets = _tmxMap.GetTilesets();
	for (int i=0; i<tsets.size(); i++) {
		Tmx::Tileset *tset = tsets[i];
		if (tset->GetName() == "mapset") {
			_mapset = tset;
		} else if (tset->GetName() == "objectset") {
			_objset = tset;
		}
	}
	
	if (!_mapset || !_objset) {
		string msg = "Failed to find the required tilesets";
		Log::Error(msg);
		throw std::runtime_error(msg);
	}
}

MapParser::~MapParser()
{
	
}


bool MapParser::ParseMap()
{
	Map::TileTemplate tileTemplate(_tmxMap.GetWidth(), _tmxMap.GetHeight());

	const vector<Tmx::Layer*> layers = _tmxMap.GetLayers();
	for (int i=0; i<layers.size(); i++) {
		Tmx::Layer *layer = layers[i];
		
		if (layer->GetName() == "background") {
			MapLayer *ml = ParseDrawableLayer(layer);
			_map->SetBackgroundLayer(ml);
			Log::Debug("Parsed background layer");
		} else if (layer->GetName() == "foreground") {
			MapLayer *ml = ParseDrawableLayer(layer);
			_map->SetForegroundLayer(ml);
			Log::Debug("Parsed foreground layer");
		} else if (layer->GetName() == "walls") {
			ParseWallLayer(layer, tileTemplate);
		} else {
			Log::Debug((string) 
				"Unable to parse: " + layer->GetName()
			);
		}
	}

	_map->SetTileTemplate(tileTemplate);
	
	return true;
}


/*
================
MapParser Private
================
*/
MapLayer* MapParser::ParseDrawableLayer(Tmx::Layer *layer)
{
	string imgFile = _mapset->GetImage()->GetSource();
	imgFile = "res/" + imgFile; 
	//imgFile = "../res/helloworld.png";
	MapLayer *mapLayer = new MapLayer(imgFile);
	
	for (int x=0; x<_tmxMap.GetWidth(); x++) {
		for (int y=0; y<_tmxMap.GetHeight(); y++) {
			int gid = layer->GetTileGid(x, y);
			if (gid == 0) 
				continue;
			
			Rect texClip = GetTextureClip(_mapset, gid);
			Rect worldClip = GetWorldClip(_mapset, x, y);
			mapLayer->AddTile(worldClip, texClip);
		}
	}
	printf("\n");
	
	mapLayer->CreateVBO();
	return mapLayer;
}

Rect MapParser::GetTextureClip(Tmx::Tileset* set, int gid)
{
	int imgw = _mapset->GetImage()->GetWidth();
	int imgh = _mapset->GetImage()->GetHeight();
	
	int xcount = imgw / set->GetTileWidth();
	int ycount = imgh / set->GetTileHeight();
	int id = gid - set->GetFirstGid();
	
	// Tiles go from left to right, top to bottom.
	//  1  2  3
	//  4  5  6
	int tilex = id % xcount;
	int tiley = id / ycount;
	tiley = ycount - tiley - 1;
	
	Rect texClip;
	texClip.x = tilex * set->GetTileWidth();
	texClip.y = tiley * set->GetTileHeight();
	texClip.w = set->GetTileWidth();
	texClip.h = set->GetTileHeight();
	
	// Divide all fields by the texture dimensions so the
	// coordinates are all in the range [0-1]
	texClip.x /= (float)imgw;
	texClip.y /= (float)imgh;
	texClip.w /= (float)imgw;
	texClip.h /= (float)imgh;
	
	return texClip;
}

Rect MapParser::GetWorldClip(Tmx::Tileset* set, int x, int y)
{
	int ycount = _tmxMap.GetHeight();
	
	float xpos = x * set->GetTileWidth();
	float ypos = (ycount - y - 1) * set->GetTileHeight();
	
	Rect wClip;
	wClip.x = xpos;
	wClip.y = ypos;
	wClip.w = set->GetTileWidth();
	wClip.h = set->GetTileHeight();
	
	return wClip;
}


void MapParser::ParseWallLayer(Tmx::Layer *layer, Map::TileTemplate &tt) 
{
	for (int x=0; x<_tmxMap.GetWidth(); x++) {
		for (int y=0; y<_tmxMap.GetHeight(); y++) {
			Tmx::MapTile tile = layer->GetTile(x, y);
			if (tile.gid == 0)
				continue;

			if (tile.id == ObjectsetTileID::ID_WALL) {
				tt.AddFlag(x, y, Map::TILE_SHADOW | Map::TILE_WALL);	
			}
		}
	}
}
