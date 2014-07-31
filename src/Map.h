#pragma once

#include <trutle/Trutle.h>
#include <Box2D/Box2D.h>
#include "Physics.h"
#include "PhysicsDebugDrawer.h"

class MapLayer;
class GameLayer;


/* Map
 */
class Map : public GameObject {
public:
	enum TileFlags {
		TILE_WALL 				= 0x1,
		TILE_VENT				= 0x2,
		TILE_SHADOW 			= 0x4,
	};

	/* TileTemplate
	 * The MapParser sets the flags of the various tiles to define
	 * which tiles cast shadow, which are walkable, etc. 
	 * The MapLoader is responsible for implemeting the template in 
	 * a GameLayer.
	 */ 
	class TileTemplate {
	public:
		TileTemplate();
		TileTemplate(int mapWidth, int mapHeight);

		void AddFlag(int x, int y, unsigned flag);

		int GetWidth() const;
		int GetHeight() const;
		unsigned GetFlags(int x, int y) const;

	private:
		int _w;
		int _h;
		vector<unsigned> _tiles;
	};


	Map(GameLayer *gameLayer);
	~Map();
	
	void SetBackgroundLayer(MapLayer *layer);
	void SetForegroundLayer(MapLayer *layer);

	void SetTileTemplate(TileTemplate tileTemplate);
	TileTemplate GetTileTemplate() const;

	b2World* GetB2World() const;
	
	
	void Update(const DeltaTime &dt);

	// The layers must be drawn individually, the default
	// Render-method does nothing
	void Render(Renderer *renderer);
	void RenderBackgroundLayer(Renderer *renderer);
	void RenderForegroundLayer(Renderer *renderer);

private:	
	friend class MapLoader;

	MapLayer *_bgLayer;
	MapLayer *_fgLayer;
	GameLayer *_gameLayer;
	TileTemplate _tileTemplate;

	b2World *_b2World;
	PhysicsDebugDrawer _debugDraw;
	vector<b2Body*> _globalBodies;

	void AddGlobalBody(b2Body *body);
};




