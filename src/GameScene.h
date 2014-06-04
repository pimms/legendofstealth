#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include <Box2D/Box2D.h>


class ShadowLayer;
class Player;

class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void LoadContent();
	void Update(const DeltaTime &dt);

private:
	b2World *_world;
	
	Layer *_gameLayer;
	ShadowLayer *_shadowLayer;

	Player *_localPlayer;
	vector<Player*> _remotePlayers;

	// Call the load methods in the order in which they appear plssss.
	// Call each method exactly once..
	void LoadInfrastructure();
	void CreateB2World();
	void LoadPlayer();
	void LoadMap();
};
