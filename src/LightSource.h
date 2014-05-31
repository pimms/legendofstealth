#pragma once

#include <trutle/Trutle.h>


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
