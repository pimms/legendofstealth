#pragma once

#include <trutle/Trutle.h>

class GameLayer;
class LightSource;
class ShadowCaster;
class Shader;

/* ShadowLayer
 * The renderer of light and shadow on the map.
 *
 * No child objects of the ShadowLayer will be rendered, and it is not
 * supposed to have any children. 
 *
 * Manage light sources and shadow casters using the Add/Remove methods.
 * It's important that deleted instances are removed from the ShadowLayer.
 */
class ShadowLayer : public Layer {
public:
	ShadowLayer(GameLayer *gameLayer);
	~ShadowLayer();

	void AddLightSource(LightSource *obj);
	void RemoveLightSource(LightSource *obj);

	void AddShadowCaster(ShadowCaster *obj);
	void RemoveShadowCaster(ShadowCaster *obj);

	void Render(Renderer *renderer);

	void SetShadowColor(Color color);

private:
	vector<LightSource*> _lightSources;
	vector<ShadowCaster*> _shadowCasters;
	Color _shadowColor;
	RenderTexture *_renderTexture;
	Shader *_shader;
	GameLayer *_gameLayer;

	void RenderLight(LightSource *source, Renderer *renderer);
	void RenderShadows(LightSource *source, Renderer *renderer);
	vector<Vec2> GetShadowVertices(LightSource *light);

	void DrawFillShadow();
	void DrawRenderTexture();

	bool ShouldDrawLight(LightSource *source);
	bool WillLightHitCaster(LightSource *source, ShadowCaster *caster);
};
