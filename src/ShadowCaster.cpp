#include "ShadowCaster.h"

#ifdef _WIN32
#include <res/Texture.h>
#else
#include <trutle/res/Texture.h>
#endif


vector<Vec2> ShadowCaster::GetShapeVertices()
{
	Rect r = GetShadowRect();

	vector<Vec2> verts;
	verts.push_back(Vec2(r.x, r.y));
	verts.push_back(Vec2(r.x+r.w, r.y));
	verts.push_back(Vec2(r.x+r.w, r.y+r.h));
	verts.push_back(Vec2(r.x, r.y+r.h));

	return verts;
}


Rect ShadowCaster::GetShadowRect() 
{
	Vec2 dim = GetTexture()->GetDimensions();
	Vec2 scale = Scale();
	
	dim.x *= scale.x;
	dim.y *= scale.y;

	Vec2 pos = Position();
	pos.x -= Pivot().x * dim.x;
	pos.y -= Pivot().y * dim.y;

	return Rect(pos.x, pos.y, dim.x, dim.y);
}
