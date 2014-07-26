#include "ShadowCaster.h"
#include <trutle/res/Texture.h>
#include <cmath>

/*
================
ShadowCaster Public
================
*/
ShadowCaster::ShadowCaster()
	:	_calcPos(-1500.f, -1500.f)
{

}


void ShadowCaster::SetShadowRectangle(Rect srect) 
{
	_localVerts.clear();
	_localVerts.push_back(Vec2(srect.x, srect.y));
	_localVerts.push_back(Vec2(srect.x + srect.w, srect.y));
	_localVerts.push_back(Vec2(srect.x + srect.w, srect.y + srect.h));
	_localVerts.push_back(Vec2(srect.x, srect.y + srect.h));

	CalculateGlobalVertices();
}


const vector<Vec2>& ShadowCaster::GetShadowShape()
{
	if (!_localVerts.size()) {
		CreateShadowVertices();
	}

	if (!_globalVerts.size() || IsPositionDirty()) {
		CalculateGlobalVertices();
	}
	
	return _globalVerts;
}

Rect ShadowCaster::GetAABB()
{
	Vec2 v = _localVerts[0];
	Rect r(v.x, v.y, 0.f, 0.f);

	// Find the bottom left point
	for (int i=1; i<_localVerts.size(); i++) {
		v = _localVerts[i];
		if (v.x < r.x)
			r.x = v.x;
		if (v.y < r.y)
			r.y = v.y;
	}

	for (int i=0; i<_localVerts.size(); i++) {
		v = _localVerts[i];
		if (v.x > r.x + r.w)
			r.w = v.x - r.x;
		if (v.y > r.y + r.h)
			r.h = v.y - r.y;
	}

	r.x += GetGameObject()->WorldPosition().x;
	r.y += GetGameObject()->WorldPosition().y;

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

	_localVerts.clear();
	_localVerts.push_back(Vec2(r.x, r.y));
	_localVerts.push_back(Vec2(r.x + r.w, r.y));
	_localVerts.push_back(Vec2(r.x + r.w, r.y + r.h));
	_localVerts.push_back(Vec2(r.x, r.y + r.h));
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


void ShadowCaster::CalculateGlobalVertices()
{
	_globalVerts.clear();
	_calcPos = GetGameObject()->WorldPosition();

	for (int i=0; i<_localVerts.size(); i++) {
		Vec2 v = _localVerts[i];
		v.x += GetGameObject()->WorldPosition().x;
		v.y += GetGameObject()->WorldPosition().y;
		_globalVerts.push_back(v);
	}
}

bool ShadowCaster::IsPositionDirty() 
{
	Vec2 p = GetGameObject()->WorldPosition();
	
	Vec2 d;
	d.x = fabs(p.x - _calcPos.x);
	d.y = fabs(p.y - _calcPos.y);

	return d.Length() > 0.001f;
}
