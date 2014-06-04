#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class LightSource : public GameObject {
public:
	LightSource();

	void SetLightDistance(float distance);
	float GetLightDistance() const;

	void SetColor(Color c);
	Color GetColor() const;

private:
	float _distance;
	Color _color;
};
