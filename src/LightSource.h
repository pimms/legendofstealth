#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class RenderTexture;


class LightSource : public GameObject {
public:
	LightSource();
	~LightSource();

	void SetLightDistance(float distance);
	float GetLightDistance() const;

	void SetColor(Color c);
	Color GetColor() const;

	void DrawRenderTexture();

protected:
	virtual void CreateLightTexture();

	float _distance;
	Color _color;
	RenderTexture *_texture;
};
