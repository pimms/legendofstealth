#include "ShadowLayer.h"


/* 
================
Public ShadowLayer
================
*/
ShadowLayer::ShadowLayer()
	: 	_shader(NULL),
		_renderTexture(NULL)
{
	_renderTexture = new RenderTexture(Vec2(1280.f, 720.f));

	_shader = new Shader("res/light.frag", "res/light.vert");
	_shader->SetUniform1i("tex", 0);
	_shader->SetUniform1f("lalpha", 0.8f);

	SetShadowColor(Color(0.f, 0.f, 0.f, 1.f));
}

ShadowLayer::~ShadowLayer()
{
	if (_renderTexture)
		delete _renderTexture;
	if (_shader)
		delete _shader;
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
	_renderTexture->BindFBO();

	glEnable(GL_STENCIL_TEST);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (int i=0; i<_lightSources.size(); i++) {
		RenderShadows(_lightSources[i], renderer);
		RenderLight(_lightSources[i], renderer);
	}

	glDisable(GL_STENCIL_TEST);

	DrawFillShadow();

	_renderTexture->UnbindFBO();

	DrawRenderTexture();
}


void ShadowLayer::SetShadowColor(Color c)
{
	_shadowColor = c;
	_shader->SetUniform4f("ulcolor", c.r, c.g, c.b, c.a);
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
		
	Vec2 p = source->GetCenterPosition();

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
	Color oc = source->GetColor();
	unsigned char c[4 * triangles];
	// Inner color, white
	c[0] = (unsigned char)(oc.r * 255.f);
	c[1] = (unsigned char)(oc.g * 255.f);
	c[2] = (unsigned char)(oc.b * 255.f);
	c[3] = (unsigned char)(oc.a * 255.f * 0.2f);
	for (int i=1; i<triangles; i++) {
		// Outer color 
		c[i * 4 + 0] = (unsigned char)(oc.r * 255.f);
		c[i * 4 + 1] = (unsigned char)(oc.g * 255.f);
		c[i * 4 + 2] = (unsigned char)(oc.b * 255.f);
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

	glColor4f(_shadowColor.r, _shadowColor.g, _shadowColor.b, 1.f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &shadowverts[0]);
	glDrawArrays(GL_QUADS, 0, shadowverts.size());
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor4ub(255, 255, 255, 255);
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

void ShadowLayer::DrawFillShadow()
{
	glColor4f(_shadowColor.r, _shadowColor.g, _shadowColor.b, _shadowColor.a);
	Vec2 r = _renderTexture->GetResolution();
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);

	float v[8] = {
		0.f,	0.f,	
		r.x,	0.f,
		r.x,	r.y,
		0.f,	r.y,	
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor4ub(255,255,255,255);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ShadowLayer::DrawRenderTexture()
{
	Vec2 r = _renderTexture->GetResolution();

	float v[8] = {
		0.f, 0.f,
		r.x, 0.f,
		r.x, r.y,
		0.f, r.y,
	};

	float t[8] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
	};

	glEnable(GL_TEXTURE_2D);
	_renderTexture->BindTex();

	_shader->Enable();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4ub(255, 255, 255, 255);

	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	_shader->Disable();
}
