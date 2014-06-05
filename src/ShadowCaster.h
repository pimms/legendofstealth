#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif


class ShadowCaster  {
public:
	virtual vector<Vec2> GetShapeVertices() = 0;
};
//