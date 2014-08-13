#pragma once

#include <trutle/Trutle.h>


/* ShadowCaster
 * Component allowing a game object to cast shadows.Only
 * boxes are currently available, but in theory any shape
 * can be used to cast shadow. 
 * 
 * The ShadowCaster provides a rendering object with the
 * shape which should cast shadow. 
 */
class ShadowCaster : public Component {
public:
	ShadowCaster();

	void SetShadowRectangle(Rect srect);

	const vector<Vec2>& GetShadowShape();
	Rect GetAABB();

private:
	vector<Vec2> _localVerts;
	vector<Vec2> _globalVerts;
	Vec2 _calcPos;	

	void CalculateGlobalVertices();
	
	virtual void CreateShadowVertices();
	virtual Rect GetShadowRect();

	bool IsPositionDirty();
};
