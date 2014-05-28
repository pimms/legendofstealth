#pragma once

#include <trutle/Trutle.h>

#include "LightSource.h"
#include "ShadowCaster.h"


class ShadowLayer : public Layer {
public:
	ShadowLayer();

	void AddLightSource(LightSource *obj);
	void RemoveLightSource(LightSource *obj);

	void AddShadowCaster(ShadowCaster *obj);
	void RemoveShadowCaster(ShadowCaster *obj);

	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);

private:
	vector<LightSource*> _lightSources;
	vector<ShadowCaster*> _shadowCasters;

	void RenderLight(LightSource *source, Renderer *renderer);
	void RenderShadows(LightSource *source, Renderer *renderer);
	vector<Vec2> GetShadowVertices(LightSource *light);
};
