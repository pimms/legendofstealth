#include "ShadowLayer.h"


/* 
================
Public ShadowLayer
================
*/
ShadowLayer::ShadowLayer()
{

}


void ShadowLayer::AddLightSource(LightSource *obj)
{
	_lightSources.push_back(obj);
}

void ShadowLayer::RemoveLightSource(LightSource *obj)
{
	for (int i=0; i<_shadowCasters.size(); i++) {
		if (_lightSources[i] == obj) {
			_lightSources.erase(_lightSources.begin()+i--);
		}
	}
}


void ShadowLayer::AddShadowCaster(ShadowCaster *obj)
{
	_shadowCasters.push_back(obj);
}

void ShadowLayer::RemoveShadowCaster(ShadowCaster *obj)
{
	for (int i=0; i<_shadowCasters.size(); i++) {
		if (_shadowCasters[i] == obj) {
			_shadowCasters.erase(_shadowCasters.begin()+i--);
		}
	}
}


void ShadowLayer::Update(const DeltaTime &dt)
{

}

void ShadowLayer::Render(Renderer *renderer)
{
	glEnable(GL_STENCIL_TEST);

	for (int i=0; i<_lightSources.size(); i++) {
		RenderShadows(_lightSources[i], renderer);
		RenderLight(_lightSources[i], renderer);
	}

	glDisable(GL_STENCIL_TEST);
}


/* 
================
Private ShadowLayer
================
*/
void ShadowLayer::RenderLight(LightSource *source, Renderer *renderer)
{
	const int triangles = 101;
	const float pi2 = 2.f * M_PI;
		
	Vec2 p = source->Position();


	// Create the vertex buffer
	float v[2 * triangles];
	v[0] = p.x;
	v[1] = p.y;
	for (int i=1; i<triangles; i++) {
		float rad = ( float(i)/float(triangles-1) ) * pi2;
		v[i*2 + 0] = p.x + source->GetLightDistance() * cos(rad);
		v[i*2 + 1] = p.y + source->GetLightDistance() * sin(rad);
	}

	v[2*(triangles-1) + 0] = v[2];
	v[2*(triangles-1) + 1] = v[3];

	// Create the color buffer
	unsigned char c[4 * triangles];
	// Inner color, white
	c[0] = 255;
	c[1] = 255;
	c[2] = 255;
	c[3] = 127;
	for (int i=1; i<triangles; i++) {
		// Outer color 
		c[i * 4 + 0] = 255;
		c[i * 4 + 1] = 255;
		c[i * 4 + 2] = 255;
		c[i * 4 + 3] = 0;
	}

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, v);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c);
	glDrawArrays(GL_TRIANGLE_FAN, 0, triangles);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void ShadowLayer::RenderShadows(LightSource *source, Renderer *renderer)
{	
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_NEVER, 0x1, 0x0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	vector<Vec2> shadowverts = GetShadowVertices(source);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4ub(0, 0, 0, 200);

	glVertexPointer(2, GL_FLOAT, 0, &shadowverts[0]);
	glDrawArrays(GL_QUADS, 0, shadowverts.size());

	glDisableClientState(GL_VERTEX_ARRAY);
}

vector<Vec2> ShadowLayer::GetShadowVertices(LightSource *source)
{
	vector<Vec2> shadow;

	for (int i=0; i<_shadowCasters.size(); i++) {
		ShadowCaster *caster = _shadowCasters[i];
		vector<Vec2> shape = caster->GetShapeVertices();
		Vec2 p = source->Position();
		
		// Push P1, P2, P2', P1' to create a quad
		for (int j=0; j<shape.size(); j++) {
			vector<Vec2> quad;
			quad.resize(4);
			for (int k=0; k<2; k++) {
				int idx = (k + j) % shape.size();
				Vec2 d;
				d.x = shape[idx].x - p.x;
				d.y = shape[idx].y - p.y;

				// Make the shadow line 1000 units long 
				float len = d.Length();
				d.x /= len;
				d.y /= len;

				d.x *= 1000.f;
				d.y *= 1000.f;

				// Push the shape vertex and the delta
				quad[k] = shape[idx];
				quad[3-k] = d;
			}

			for (int k=0; k<4; k++)
				shadow.push_back(quad[k]);
		}
	}

	return shadow;
}


