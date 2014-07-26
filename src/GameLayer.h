#pragma once

#include <trutle/Trutle.h>

class Map;
class Player;
class ShadowLayer;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();
	
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);


	ShadowLayer* GetShadowLayer();
	
private:
	Map *_map;
	ShadowLayer *_shadowLayer;
	Layer *_uiLayer;

	Player *_player;
};
