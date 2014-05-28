#pragma once

#include <trutle/Trutle.h>


class LightSource : public GameObject {
public:
	void SetLightDistance(float distance);
	float GetLightDistance() const;

private:
	float _distance;
};
