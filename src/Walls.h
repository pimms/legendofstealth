#pragma once
#include "Entity.h"
#include "ShadowCaster.h"
class Walls :public Entity , public ShadowCaster
{
public:
	Walls(b2World *world, string texture, Vec2 scale, Vec2 pos);

	virtual vector<Vec2> GetShapeVertices();
	Rect GetShadowRect();
	Rect GetAABB();

	void Render(Renderer *renderer);
};

