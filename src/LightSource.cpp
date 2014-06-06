#include "LightSource.h"
#include "RenderTexture.h"
#include "Entity.h"


RenderTexture* LightSource::_preloadTex = NULL;
int LightSource::_preloadRefs = 0;

/*
================
LightSource Public
================
*/
LightSource::LightSource(bool usePreloaded)
	:	_color(1.f, 1.f, 1.f, 1.f),
		_texture(NULL),
		_distance(100.f),
		_usingPreloaded(usePreloaded)
{
	if (!usePreloaded) {
		CreateLightTexture();
	} else {
		if (!_preloadTex) {
			CreateLightTexture();
			_preloadTex = _texture;
		} else {
			_texture = _preloadTex;
		}
	}
}

LightSource::~LightSource()
{
	if (!_usingPreloaded) {
		if (_texture)
			delete _texture;
	} else {
		_preloadRefs--;
		if (!_preloadRefs) {
			delete _preloadTex;
			_preloadTex = NULL;
		}
	}
}

void LightSource::SetLightDistance(float distance)
{
	_distance = distance;

	CreateLightTexture();
}

float LightSource::GetLightDistance() const
{
	return _distance;
}


void LightSource::SetColor(Color c)
{
	_color = c;
	CreateLightTexture();
}

Color LightSource::GetColor() const
{
	return _color;
}


void LightSource::DrawRenderTexture()
{
	Vec2 wp = WorldPosition();

	glPushMatrix();
	glTranslatef(wp.x, wp.y, 0.f);
	glRotatef(WorldRotation(), 0.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);

	_texture->BindTex();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float v[] = {
		-_distance, -_distance,
		 _distance, -_distance,
		 _distance,  _distance,
		-_distance,  _distance,
	};

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

	_texture->UnbindTex();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


/*
================
LightSource Protected
================
*/
void LightSource::CreateLightTexture()
{
	if (_texture)
		delete _texture;

	_texture = new RenderTexture(Vec2(_distance*2, _distance*2));
	_texture->BindFBO();

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	const int triangles = 101;
	const float pi2 = 2.f * M_PI;
		
	Vec2 p = Vec2(_distance, _distance);

	// Create the vertex buffer
	float v[2 * triangles];
	v[0] = p.x;
	v[1] = p.y;
	for (int i=1; i<triangles; i++) {
		float rad = ( float(i)/float(triangles-1) ) * pi2;
		v[i*2 + 0] = p.x + GetLightDistance() * cos(rad);
		v[i*2 + 1] = p.y + GetLightDistance() * sin(rad);
	}

	v[2*(triangles-1) + 0] = v[2];
	v[2*(triangles-1) + 1] = v[3];

	// Create the color buffer
	Color oc = GetColor();
	unsigned char c[4 * triangles];
	// Inner color, white
	c[0] = (unsigned char)(oc.r * 255.f);
	c[1] = (unsigned char)(oc.g * 255.f);
	c[2] = (unsigned char)(oc.b * 255.f);
	c[3] = (unsigned char)(oc.a * 255.f);
	for (int i=1; i<triangles; i++) {
		// Outer color 
		c[i * 4 + 0] = (unsigned char)(oc.r * 255.f);
		c[i * 4 + 1] = (unsigned char)(oc.g * 255.f);
		c[i * 4 + 2] = (unsigned char)(oc.b * 255.f);
		c[i * 4 + 3] = 0;
	}


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, v);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c);
	glDrawArrays(GL_TRIANGLE_FAN, 0, triangles);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	_texture->UnbindFBO();
}



/*
================
ConeLightSource Public
================
*/
ConeLightSource::ConeLightSource()
	:	_angle(120.f)
{

}

void ConeLightSource::CreateLightTexture()
{
	if (_texture)
		delete _texture;

	if (_angle < 9.9f) {
		string msg = "Cannot create ConeLightSource with < 10 degree angle";
		Log::Error(msg);
		throw std::runtime_error(msg);
		return;
	}

	_texture = new RenderTexture(Vec2(_distance*2, _distance*2));
	_texture->BindFBO();

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	const int triangles = (int)_angle/3 + 1;
	const float pi2 = 2.f * M_PI;
		
	Vec2 p = Vec2(_distance, _distance);

	// Create the vertex buffer
	std::vector<float> v(2 * triangles);
	v[0] = p.x;
	v[1] = p.y;
	for (int i=1; i<triangles; i++) {
		int degrees = i * 3;
		degrees -= (int)(_angle / 2.f);
		float radians = Deg2Rad((float)degrees);

		v[i*2 + 0] = p.x + GetLightDistance() * cos(radians);
		v[i*2 + 1] = p.y + GetLightDistance() * sin(radians);
	}

	v[2*(triangles-1) + 0] = v[2];
	v[2*(triangles-1) + 1] = v[3];

	// Create the color buffer
	Color oc = GetColor();
	std::vector<unsigned char> c(4 * triangles);
	// Inner color, white
	c[0] = (unsigned char)(oc.r * 255.f);
	c[1] = (unsigned char)(oc.g * 255.f);
	c[2] = (unsigned char)(oc.b * 255.f);
	c[3] = (unsigned char)(oc.a * 255.f);
	for (int i=1; i<triangles; i++) {
		// Outer color 
		c[i * 4 + 0] = (unsigned char)(oc.r * 255.f);
		c[i * 4 + 1] = (unsigned char)(oc.g * 255.f);
		c[i * 4 + 2] = (unsigned char)(oc.b * 255.f);
		c[i * 4 + 3] = 0;
	}


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, &v[0]);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &c[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, triangles);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	_texture->UnbindFBO();
}
