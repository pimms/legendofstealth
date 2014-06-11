#pragma once

#include <trutle/Trutle.h>

class Map;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();
	
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);
	
private:
	Map *_map;
	Layer *_shadowLayer;
	Layer *_uiLayer;
};