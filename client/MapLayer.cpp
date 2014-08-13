#include "MapLayer.h"
#include <GL/gl.h>
#include <trutle/res/Texture.h>

/*
================
MapLayer Public
================
*/
MapLayer::MapLayer(string textureFile)
	:	_texBuf(0),
		_vertBuf(0),
		_numTiles(0)
{
	LoadTexture(textureFile);
}

MapLayer::~MapLayer()
{
	DestroyBuffers();
}


void MapLayer::AddTile(Rect worldClip, Rect texClip)
{
	_vertices.push_back(worldClip.x);
	_vertices.push_back(worldClip.y);
	
	_vertices.push_back(worldClip.x + worldClip.w);
	_vertices.push_back(worldClip.y);
	
	_vertices.push_back(worldClip.x + worldClip.w);
	_vertices.push_back(worldClip.y + worldClip.h);
	
	_vertices.push_back(worldClip.x);
	_vertices.push_back(worldClip.y + worldClip.h);
	
	
	_texCoords.push_back(texClip.x);
	_texCoords.push_back(texClip.y);
	
	_texCoords.push_back(texClip.x + texClip.w);
	_texCoords.push_back(texClip.y);
	
	_texCoords.push_back(texClip.x + texClip.w);
	_texCoords.push_back(texClip.y + texClip.h);
	
	_texCoords.push_back(texClip.x);
	_texCoords.push_back(texClip.y + texClip.h);
	
	_numTiles++;
	_tileSize = Vec2(texClip.w, texClip.h);
}


void MapLayer::CreateVBO()
{
	if (!_numTiles) {
		Log::Warning((string)"MapLayer::CreateVBO() called " +
					 "on a layer with no defined tiles");
		return;
	}
	
	if (!_vertBuf || !_texBuf) {
		CreateBuffers();
	}	
	
	FillBuffers();
}


void MapLayer::Render(Renderer* renderer)
{
	if (!_numTiles)
		return;
	
	glPushMatrix();
	
	glEnable(GL_TEXTURE_2D);
	GetTexture()->Bind();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vertBuf);
	glVertexPointer(2, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, _texBuf);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	
	glDrawArrays(GL_QUADS, 0, _numTiles * 4);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	Renderer::PrintOpenGLErrors("MapLayer::Render()");
}


/*
================
MapLayer Private 
================
*/
void MapLayer::DestroyBuffers()
{
	if (_texBuf) {
		glDeleteBuffers(1, &_texBuf);
		_texBuf = 0;
	}
	
	if (_vertBuf) {
		glDeleteBuffers(1, &_vertBuf);
		_vertBuf = 0;
	}	
}

void MapLayer::CreateBuffers()
{
	if (_texBuf || _vertBuf) {
		DestroyBuffers();
	}
	
	glGenBuffers(1, &_vertBuf);
	glGenBuffers(1, &_texBuf);
	
	if (!_texBuf || !_vertBuf) {
		Log::Error("Unable to create vertex buffers");
	}
	
	Renderer::PrintOpenGLErrors("VBO creation");
}

void MapLayer::FillBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, _texBuf);
	glBufferData(GL_ARRAY_BUFFER, 
				 sizeof(GLfloat)*_texCoords.size(),
				 &_texCoords[0],
				 GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vertBuf);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(GLfloat)*_vertices.size(),
				 &_vertices[0],
				 GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	Renderer::PrintOpenGLErrors("MapLayer::FillBuffers()");
}
