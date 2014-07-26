#pragma once


#include <trutle/Trutle.h>


const float PLAYER_DEFAULT_SPEED = 150.f;


class MoveComponent : public Component {
public:
	MoveComponent();
	void Update(const DeltaTime &dt);

private:
	float _speed;
};


class Player : public GameObject {
public:
	Player();
	~Player();

private:

};
