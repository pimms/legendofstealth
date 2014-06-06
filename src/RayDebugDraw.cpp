#include "RayDebugDraw.h"
#include "Entity.h"


void RayDebugDraw::Render(Renderer *renderer)
{
	glPushMatrix();
	renderer->ApplyTransform(GetParentLayer());

	Vec2 p1 = ToVec2(_p1);
	Vec2 p2 = ToVec2(_p2);

	glColor4ub(255, 0, 0, 255);
	glLineWidth(5.f);

	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();

	glColor4ub(255, 255, 255, 255);

	glPopMatrix();
}

void RayDebugDraw::SetRay(b2Vec2 p1, b2Vec2 p2)
{
	_p1 = p1;
	_p2 = p2;
}
