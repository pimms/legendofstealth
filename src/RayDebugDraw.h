#pragma once

#include <trutle/Trutle.h>
#include <Box2D/Box2D.h>


class RayDebugDraw : public GameObject {
public:
	void Render(Renderer *renderer);
	void SetRay(b2Vec2 p1, b2Vec2 p2);

private:
	b2Vec2 _p1;
	b2Vec2 _p2;
};
