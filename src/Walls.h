#pragma once
#include "Entity.h"
#include "ShadowCaster.h"
class Walls :public Entity , public ShadowCaster
{
public:
	Walls(b2World *world, string texture);
	Rect GetShadowRect();

	void Render(Renderer *renderer);

	virtual vector<Vec2> GetShapeVertices();
	virtual Rect GetAABB();
};

