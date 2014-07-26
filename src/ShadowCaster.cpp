#include "ShadowCaster.h"
#include <trutle/res/Texture.h>

/*
================
ShadowCaster Public
================
*/
const vector<Vec2>& ShadowCaster::GetShadowShape()
{
	if (!_shadowVerts.size()) {
		CreateShadowVertices();
	}
	
	return _shadowVerts;
}

Rect ShadowCaster::GetAABB() const 
{
	Vec2 v = _shadowVerts[0];
	Rect r(v.x, v.y, 0.f, 0.f);

	// Find the bottom left point
	for (int i=1; i<_shadowVerts.size(); i++) {
		v = _shadowVerts[i];
		if (v.x < r.x)
			r.x = v.x;
		if (v.y < r.y)
			r.y = v.y;
	}

	for (int i=0; i<_shadowVerts.size(); i++) {
		v = _shadowVerts[i];
		if (v.x > r.x + r.w)
			r.w = v.x - r.x;
		if (v.y > r.y + r.h)
			r.h = v.y - r.y;
	}

	return r;
}


/*
================
ShadowCaster Private 
================
*/
void ShadowCaster::CreateShadowVertices()
{
	Rect r = GetShadowRect();

	_shadowVerts.clear();
	_shadowVerts.push_back(Vec2(r.x, r.y));
	_shadowVerts.push_back(Vec2(r.x + r.w, r.y));
	_shadowVerts.push_back(Vec2(r.x + r.w, r.y + r.h));
	_shadowVerts.push_back(Vec2(r.x, r.y + r.h));
}

Rect ShadowCaster::GetShadowRect()
{
	GameObject *go = GetGameObject();
	Vec2 dim = go->GetTexture()->GetDimensions();
	Vec2 scale = Scale();

	dim.x *= scale.x;
	dim.y *= scale.y;

	Vec2 pos = GetGameObject()->WorldPosition();
	pos.x -= Pivot().x * dim.x;
	pos.y -= Pivot().y * dim.y;

	return Rect(pos.x, pos.y, dim.x, dim.y);
}
