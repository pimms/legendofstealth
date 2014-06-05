#pragma once
#include "Entity.h"
#include "ShadowCaster.h"
class Walls :public Entity , public ShadowCaster
{
public:
	Walls(b2World *world, string texture);
	virtual vector<Vec2> GetShapeVertices();
	Rect Walls::GetShadowRect();
};

//