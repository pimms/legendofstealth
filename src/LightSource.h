#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class RenderTexture;


class LightSource : public GameObject {
public:
	LightSource(bool usePreloaded = false);
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

	bool _usingPreloaded;

	static RenderTexture *_preloadTex;
	static int _preloadRefs;
};


class ConeLightSource : public LightSource {
public:
	ConeLightSource();

protected:
	void CreateLightTexture();

	float _angle;
};
