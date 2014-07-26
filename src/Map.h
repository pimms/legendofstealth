#pragma once

#include <trutle/Trutle.h>

class MapLayer;

/* Map
 */
class Map : public GameObject {
public:
	Map();
	~Map();
	
	void SetBackgroundLayer(MapLayer *layer);
	void SetForegroundLayer(MapLayer *layer);
	
	
	// The layers must be drawn individually, the default
	// Render-method does nothing
	void Render(Renderer *renderer);
	void RenderBackgroundLayer(Renderer *renderer);
	void RenderForegroundLayer(Renderer *renderer);

private:	
	MapLayer *_bgLayer;
	MapLayer *_fgLayer;
	
};
