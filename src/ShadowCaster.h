#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif


class ShadowCaster : public GameObject {
public:
	vector<Vec2> GetShapeVertices();
	
private:
	Rect GetShadowRect();	

};
