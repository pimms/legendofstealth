#pragma once


#include <trutle/Trutle.h>
#include "Physics.h"


const float PLAYER_DEFAULT_SPEED = 400.f;


class Map;
class GameLayer;


/* MoveComponent
 * Component responsible for moving the player around
 */
class MoveComponent : public Component {
public:
	MoveComponent();
	void Update(const DeltaTime &dt);

	void SetBody(b2Body *body);

private:
	float _speed;
	b2Body *_body;
};


/* AimComponent
 * Component responsible for rotating the player towards the mouse
 */
class AimComponent : public Component {
public:
	AimComponent();
	void Update(const DeltaTime &dt);
};


/* Player
 * A hollow shell dragged around by a handful of dedicated components.
 */
class Player : public GameObject {
public:
	Player(Map *map, GameLayer *gameLayer);
	~Player();

	void MoveToB2Body();

private:
	b2Body *_body;

	void CreatePhysics(b2World *world);

};
