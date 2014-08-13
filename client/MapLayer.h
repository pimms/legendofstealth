#pragma once

#include <trutle/Trutle.h>


/* MapLayer
 * A drawable layer in a Map. The MapLayer can only be composed
 * from a single texture. The 
 */
class MapLayer : public GameObject {
public:
	MapLayer(string textureFile);
	~MapLayer();
	
	void AddTile(Rect worldClip, Rect texClip);
	
	// Create a VBO from the current data. Must be called after
	// adding a tile for it to be drawn.
	void CreateVBO();
	
	void Render(Renderer *renderer);

private:	
	vector<GLfloat> _texCoords;
	vector<GLfloat> _vertices;
	int _numTiles;
	Vec2 _tileSize;
	
	GLuint _texBuf;
	GLuint _vertBuf;
	
	
	void DestroyBuffers();
	void CreateBuffers();
	void FillBuffers();
};