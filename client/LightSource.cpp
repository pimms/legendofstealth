#include "LightSource.h"


/*
================
LightSource::Properties Public
================
*/
LightSource::Properties::Properties()
{
	initialized = false;
	radius = 0.f;
	color = Color(0.f, 0.f, 0.f, 0.f);
}

LightSource::Properties::Properties(float r, Color c)
{
	initialized = true;
	radius = r;
	color = c;
}

bool LightSource::Properties::IsInitialized()
{
	return initialized;
}


/*
================
LightTexture Public
================
*/
RenderTexture* LightTexture::GetSharedTexture()
{
	if (!_renderTexture)
		CreateSharedTexture();
	return _renderTexture;
}

int LightTexture::GetRetainCount()
{
	return _refs;
}


void LightTexture::RetainSharedTexture()
{
	_refs++;
}

void LightTexture::ReleaseSharedTexture()
{
	if (_refs)
		_refs--;
	if (!_refs && _renderTexture) 
		DeleteSharedTexture();
}


/*
================
LightTexture Private 
================
*/
RenderTexture* LightTexture::_renderTexture = NULL;
int LightTexture::_refs = 0;
const float LightTexture::c_lightRadius = 256.f;

void LightTexture::CreateSharedTexture()
{
	if (!_renderTexture) {
		Vec2 res(c_lightRadius*2.f, c_lightRadius*2.f);
		_renderTexture = new RenderTexture(res);
	}
		
	_renderTexture->BindFBO();
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	const int triangles = 101;
	const float pi2 = 2.f * M_PI;
		
	Vec2 p = Vec2(c_lightRadius, c_lightRadius);

	// Create the vertex buffer
	float v[2 * triangles];
	v[0] = p.x;
	v[1] = p.y;
	for (int i=1; i<triangles; i++) {
		float rad = ( float(i)/float(triangles-1) ) * pi2;
		v[i*2 + 0] = p.x + c_lightRadius * cos(rad);
		v[i*2 + 1] = p.y + c_lightRadius * sin(rad);
	}

	v[2*(triangles-1) + 0] = v[2];
	v[2*(triangles-1) + 1] = v[3];

	// Create the color buffer
	unsigned char c[4 * triangles];
	// Inner color
	c[0] = 255;
	c[1] = 255;
	c[2] = 255;
	c[3] = 255;
	for (int i=1; i<triangles; i++) {
		// Outer color 
		c[i * 4 + 0] = 255;
		c[i * 4 + 1] = 255;
		c[i * 4 + 2] = 255;
		c[i * 4 + 3] = 0;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, v);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c);
	glDrawArrays(GL_TRIANGLE_FAN, 0, triangles);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	_renderTexture->UnbindFBO();
}

void LightTexture::DeleteSharedTexture()
{
	if (_renderTexture)
		delete _renderTexture;
}


/*
================
LightSource Public
================
*/
LightSource::LightSource()
	:	_texture(NULL),
		_sharedTexture(NULL)
{
	
}

LightSource::~LightSource()
{
	ReleaseSharedTexture();

	if (_texture != NULL) {
		_texture->Release();
	}
}


void LightSource::SetLightProperties(float lightDistance, Color color)
{
	_properties = Properties(lightDistance, color);
	UseSharedTexture();
}


void LightSource::SetTexture(Texture* texture)
{
	ReleaseSharedTexture();

	_texture = texture;
	_texture->Retain();
	
	// Invalidate the properties (the radius is still used however)
	_properties.initialized = false;
	_properties.radius = _texture->GetDimensions().x;
}


LightSource::Properties LightSource::GetLightProperties() const
{
	return _properties;
}

float LightSource::GetLightRadius() const
{
	return _properties.radius;
}

Color LightSource::GetLightColor() const 
{
	return _properties.color;
}

Rect LightSource::GetAABB() const 
{
	Vec2 p = Position();
	Rect r;
	float rad = GetLightRadius();

	r.x = p.x - rad;
	r.y = p.y - rad;
	r.w = rad * 2;
	r.h = rad * 2;

	return r;
}


void LightSource::RenderLight(Renderer *renderer)
{
	GLuint tex = 0;
	bool shared = false;

	if (IsUsingSharedTexture()) {
		shared = true;
		tex = _sharedTexture->GetTex();
	} else if (_texture) {
		shared = false;
		tex = _texture->GetTextureID();
	} else { 
		return;
	}
	
	GameObject *go = GetGameObject();
	Vec2 wp = go->WorldPosition();

	glPushMatrix();
	glTranslatef(wp.x, wp.y, 0.f);
	glRotatef(go->WorldRotation(), 0.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float *v = NULL; 
	
	if (shared) {
		v = new float[8] {
			-_properties.radius, -_properties.radius,
			 _properties.radius, -_properties.radius,
			 _properties.radius,  _properties.radius,
			-_properties.radius,  _properties.radius,
		};
	} else {
		Vec2 d = _texture->GetDimensions();
		v = new float[8] {
			0.f, -d.y / 2.f,
			d.x, -d.y / 2.f,
			d.x,  d.y / 2.f,
			0.f,  d.y / 2.f,
		};
	}

	float t[] = {
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
	};

	glColor4ub(255, 255, 255, 255);

	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	delete[] v;
}


/*
================
LightSource Private 
================
*/
void LightSource::UseSharedTexture()
{
	_texture = NULL;

	if (!IsUsingSharedTexture()) {
		_sharedTexture = LightTexture::GetSharedTexture();
		LightTexture::RetainSharedTexture();
	}
}

void LightSource::ReleaseSharedTexture()
{
	if (IsUsingSharedTexture()) {
		LightTexture::ReleaseSharedTexture();
		_sharedTexture = NULL;
	}
}

bool LightSource::IsUsingSharedTexture()
{
	return (_sharedTexture);
}
