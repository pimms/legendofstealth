#include "Walls.h"

#ifdef _WIN32
#include <res/Texture.h>
#else
#include <trutle/res/Texture.h>
#endif

Walls::Walls(b2World *world, string texture, Vec2 scale, Vec2 pos) 
	: Entity(world), ShadowCaster()
{
	LoadTexture(texture);
	Scale() = scale;
	Position() = pos;

	Vec2 dim = GetTexture()->GetDimensions();
	dim.x *= scale.x;
	dim.y *= scale.y;
	Position().x += dim.x / 2.f;
	Position().y += dim.y / 2.f;

	CreateSquareBody(b2_staticBody);
}


vector<Vec2> Walls::GetShapeVertices()
{
	Rect r = GetShadowRect();

	vector<Vec2> verts;
	verts.push_back(Vec2(r.x, r.y));
	verts.push_back(Vec2(r.x + r.w, r.y));
	verts.push_back(Vec2(r.x + r.w, r.y + r.h));
	verts.push_back(Vec2(r.x, r.y + r.h));

	return verts;
}

Rect Walls::GetShadowRect()
{
	Vec2 dim = GetTexture()->GetDimensions();
	Vec2 scale = Scale();

	dim.x *= scale.x;
	dim.y *= scale.y;

	Vec2 pos = WorldPosition();
	pos.x -= Pivot().x * dim.x;
	pos.y -= Pivot().y * dim.y;

	return Rect(pos.x, pos.y, dim.x, dim.y);
}

Rect Walls::GetAABB()
{
	return GetShadowRect();
}

void Walls::Render(Renderer *renderer)
{
	// Don't render off-screen walls
	Vec2 dim = GetTexture()->GetDimensions();
	Rect r(Position().x, Position().y, dim.x, dim.y);

	Layer *layer = GetParentLayer();
	Vec2 lp = layer->Position();
	Rect s(-lp.x, -lp.y, 1280.f, 720.f);

	if (r.Overlaps(s)) {
		Entity::Render(renderer);
	}
}
