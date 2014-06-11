#include "MapParser.h"
#include "Map.h"
#include "MapLayer.h"


/*
================
MapParser Public
================
*/
MapParser::MapParser(string mapFile)
	:	_mapFile(mapFile),
		_mapset(NULL),
		_objset(NULL)
{
	// Locate mapset and objectset
	_map.ParseFile(mapFile);
	
	if (_map.HasError()) {
		string msg = "Failed to parse map: ";
		msg += _map.GetErrorText();
		Log::Error(msg);
		throw std::runtime_error(msg);
	}
	
	vector<Tmx::Tileset*> tsets = _map.GetTilesets();
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


Map* MapParser::ParseMap()
{
	Map *gameMap = new Map();
	
	const vector<Tmx::Layer*> layers = _map.GetLayers();
	for (int i=0; i<layers.size(); i++) {
		Tmx::Layer *layer = layers[i];
		
		if (layer->GetName() == "background") {
			MapLayer *ml = ParseMapLayer(layer);
			gameMap->SetBackgroundLayer(ml);
			Log::Debug("Parsed background layer");
		} else if (layer->GetName() == "foreground") {
			MapLayer *ml = ParseMapLayer(layer);
			gameMap->SetForegroundLayer(ml);
			Log::Debug("Parsed foreground layer");
		} else {
			Log::Debug((string) 
				"Unable to parse: " + layer->GetName()
			);
		}
	}
	
	return gameMap;
}


/*
================
MapParser Private
================
*/
MapLayer* MapParser::ParseMapLayer(Tmx::Layer *layer)
{
	string imgFile = _mapset->GetImage()->GetSource();
	imgFile = "../res/" + imgFile; 
	//imgFile = "../res/helloworld.png";
	MapLayer *mapLayer = new MapLayer(imgFile);
	
	for (int x=0; x<_map.GetWidth(); x++) {
		for (int y=0; y<_map.GetHeight(); y++) {
			int gid = layer->GetTileGid(x, y);
			
			printf("%i", gid);
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
	int ycount = _map.GetHeight();
	
	float xpos = x * set->GetTileWidth();
	float ypos = (ycount - y - 1) * set->GetTileHeight();
	
	Rect wClip;
	wClip.x = xpos;
	wClip.y = ypos;
	wClip.w = set->GetTileWidth();
	wClip.h = set->GetTileHeight();
	
	return wClip;
}
