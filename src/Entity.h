#pragma once

#ifdef _WIN32
#include <Trutle.h>
#else
#include <trutle/Trutle.h>
#endif
#include <Box2D/Box2D.h>


#define PTM 32
Vec2 ToVec2(b2Vec2 bvec);
b2Vec2 Tob2Vec2(Vec2 vec);

float Deg2Rad(float deg);
float Rad2Deg(float rad);


class Entity : public GameObject {
public:
	Entity(b2World *world);
	~Entity();

	void Update(const DeltaTime &dt);
	
protected:
	b2World *_world;
	b2Body *_body;

	void CreateSquareBody(b2BodyType type = b2_dynamicBody);
};
