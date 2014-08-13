#include "MapParser.h"
#include "MapLayer.h"
#include "GameLayer.h"


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
				"Unable to parse Layer: " + layer->GetName()
			);
		}
	}

	for (int i=0; i<_tmxMap.GetNumObjectGroups(); i++) {
		const Tmx::ObjectGroup *objGroup = _tmxMap.GetObjectGroup(i);

		if (objGroup->GetName() == "lights") {
			ParseLightLayer(objGroup);
		} else {
			Log::Debug((string)
				"Unable to parse ObjectGroup: " + objGroup->GetName()
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
	tiley = ycount - tiley;
	
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
	float xpos = x * set->GetTileWidth();
	float ypos = FlipY(y) * set->GetTileHeight();
	
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
		for (int mapy=0; mapy<_tmxMap.GetHeight(); mapy++) {
			Tmx::MapTile tile = layer->GetTile(x, mapy);
			if (tile.gid == 0)
				continue;

			int y = FlipY(mapy);

			if (tile.id == ObjectsetTileID::ID_WALL) {
				tt.AddFlag(x, y, Map::TILE_SHADOW | Map::TILE_WALL);	
			} else if (tile.id == ObjectsetTileID::ID_VENT) {
				tt.AddFlag(x, y, Map::TILE_VENT);
			}
		}
	}
}


void MapParser::ParseLightLayer(const Tmx::ObjectGroup *group)
{
	for (int i=0; i<group->GetNumObjects(); i++) {
		const Tmx::Object *obj = group->GetObject(i);
		string type = obj->GetType();

		/* Handle all lights the same way - read from the Ellipse */
		const Tmx::Ellipse *ellipse = obj->GetEllipse();
		if (ellipse == NULL) {
			string msg = "Non-elliptic object in Layer 'lights' (name: " 
				+ obj->GetName() + ", type: " + type + ")";
			Log::Warning(msg);
			continue;
		}

		// The light shape must be a perfect circle
		float rx = ellipse->GetRadiusX();
		float ry = ellipse->GetRadiusY();
		if (abs(rx - ry) > 0.01) {
			string msg = "Non-perfect circle light in Layer 'lights' (name: " 
				+ obj->GetName() + ", type: " + type + ")";
			Log::Warning(msg);
			continue;
		}

		// Read the color
		Color color;
		const Tmx::PropertySet &set = obj->GetProperties();
		
		if (set.HasProperty("r") && set.HasProperty("g") &&
			set.HasProperty("b") && set.HasProperty("a")) {
			color.r = set.GetFloatProperty("r");
			color.g = set.GetFloatProperty("g");
			color.b = set.GetFloatProperty("b");
			color.a = set.GetFloatProperty("a");
		} else {
			Log::Warning("No color defined in light with name: '" + obj->GetName() + "'");
			color = Color(1.f, 1.f, 1.f, 0.5f);
		}

		
		Vec2 pos(ellipse->GetCenterX(), FlipPixelY(ellipse->GetCenterY()));
		LightSource::Properties props;
		props.radius = rx;
		props.color = color;
		
		_map->GetGameLayer()->AddLight(props, pos, type);
	}
}


int MapParser::FlipY(int mapY) 
{
	int ycount = _tmxMap.GetHeight();
	return (ycount - mapY - 1);
}

int MapParser::FlipPixelY(int mapY) 
{
	int height = _tmxMap.GetHeight() * _tmxMap.GetTileHeight();
	return (height - mapY);
}
