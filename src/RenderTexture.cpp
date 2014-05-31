#include "RenderTexture.h"


RenderTexture::RenderTexture(const Vec2 resolution) {
	_res	= resolution;
	_fbo	= 0;
	_rbo	= 0;
	_tex	= 0;

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, (GLsizei)_res.x, 
				(GLsizei)_res.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
							GL_TEXTURE_2D, _tex, 0);

	// Create the renderbuffer
	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
						  (GLsizei)_res.x, (GLsizei)_res.y);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
							  GL_RENDERBUFFER, _rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

RenderTexture::~RenderTexture() {
	if (_fbo) {
		glDeleteFramebuffers(1, &_fbo);
	}

	if (_rbo) {
		glDeleteRenderbuffers(1, &_rbo);
	}

	if (_tex) {
		glDeleteTextures(1, &_tex);
	}
}

void RenderTexture::BindFBO() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_VIEWPORT_BIT);

	glViewport(0,0, (GLsizei)_res.x, (GLsizei)_res.y);
	glOrtho(0.0, (GLdouble)_res.x, 0.0, (GLdouble)_res.y, 0, 1);
}

void RenderTexture::UnbindFBO() const {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::BindTex() const {
	glBindTexture(GL_TEXTURE_2D, _tex);
}

void RenderTexture::UnbindTex() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint RenderTexture::GetTex() const {
	return _tex;
}

Vec2 RenderTexture::GetResolution() const
{
	return _res;
}
