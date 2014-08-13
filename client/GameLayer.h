#pragma once

#include <trutle/Trutle.h>
#include "LightSource.h"

class Map;
class Player;
class ShadowLayer;


/* CameraComponent
 * The CamerComponent is added to the GameLayer in order to follow the
 * local Player. 
 */
class CameraComponent : public Component {
public:
	void SetPlayer(Player *player);

	void Update(const DeltaTime &dt);

private:
	Player *_player;
};


/* GameLayer
 * The root layer in the scene.
 */
class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();
	
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);

	void AddLight(LightSource::Properties lightProps, Vec2 pos, string type);

	ShadowLayer* GetShadowLayer();
	
private:
	Map *_map;
	ShadowLayer *_shadowLayer;
	Layer *_uiLayer;

	Player *_player;
};
