#pragma once

#include <trutle/Trutle.h>

class Map;
class GameLayer;

class GameScene : public Scene {
public:
	GameScene();
	
	void LoadContent();
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);
	
private:
	GameLayer *_gameLayer;
};