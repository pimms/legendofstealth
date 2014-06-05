#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include "LightSource.h"
#include "ShadowCaster.h"
#include "RenderTexture.h"
#include "Shader.h"


class ShadowLayer : public Layer {
public:
	ShadowLayer(Layer *siblingLayer);
	~ShadowLayer();

	void AddLightSource(LightSource *obj);
	void RemoveLightSource(LightSource *obj);

	void AddShadowCaster(ShadowCaster *obj);
	void RemoveShadowCaster(ShadowCaster *obj);

	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);

	void SetShadowColor(Color color);

private:
	vector<LightSource*> _lightSources;
	vector<ShadowCaster*> _shadowCasters;
	Color _shadowColor;
	RenderTexture *_renderTexture;
	Shader *_shader;
	Layer *_sibling;

	void RenderLight(LightSource *source, Renderer *renderer);
	void RenderShadows(LightSource *source, Renderer *renderer);
	vector<Vec2> GetShadowVertices(LightSource *light);

	void DrawFillShadow();
	void DrawRenderTexture();
};
