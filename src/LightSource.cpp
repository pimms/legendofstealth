#include "LightSource.h"


LightSource::LightSource()
	:	_color(1.f, 1.f, 1.f, 1.f)
{

}

void LightSource::SetLightDistance(float distance)
{
	_distance = distance;
}

float LightSource::GetLightDistance() const
{
	return _distance;
}


void LightSource::SetColor(Color c)
{
	_color = c;
}

Color LightSource::GetColor() const
{
	return _color;
}

