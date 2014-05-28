#pragma once

#include <trutle/Trutle.h>


class ShadowCaster : public GameObject {
public:
	vector<Vec2> GetShapeVertices();
	
private:
	Rect GetShadowRect();	

};
