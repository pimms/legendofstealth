#include <trutle/Trutle.h>
#include "ShadowLayer.h"
#include "LightSource.h"
#include "ShadowCaster.h"


class MoveComponent : public Component {
public:
	void Update(const DeltaTime &dt)
	{
		const InputState *in = GetGameObject()->GetInputState();
		if (in->IsKeyDown(SDLK_h))
			Position().x -= 150 * dt.dt;
		if (in->IsKeyDown(SDLK_j))
			Position().y -= 150 * dt.dt;
		if (in->IsKeyDown(SDLK_k))
			Position().y += 150 * dt.dt;
		if (in->IsKeyDown(SDLK_l))
			Position().x += 150 * dt.dt;
	}

};


int main(int argc, char *argv[]) 
{
	App app;
	app.Initialize(argc, argv);

	Scene *scene = new Scene;
	app.GetController()->SetScene(scene);

	Layer *layer = new Layer;
	scene->AddLayer(layer);

	GameObject *background = new GameObject;
	background->LoadTexture("res/bg.png");
	background->Position() = Vec2(320.f, 240.f);
	layer->AddChild(background);

	ShadowLayer *shadow = new ShadowLayer;
	scene->AddLayer(shadow);

	// Create a couple of lights
	for (int i=0; i<3; i++) {
		LightSource *light = new LightSource;
		light->SetLightDistance(150.f);
		light->LoadTexture("res/light.png");
		light->Position() = Vec2(100.f + 120*i, 200.f);
		layer->AddChild(light);
		shadow->AddLightSource(light);
		if (!i)
			AddComponent<MoveComponent>(light);
	}

	// Create a couple of boxes
	for (int i=0; i<5; i++) {
		ShadowCaster *box = new ShadowCaster;
		box->LoadTexture("res/box.png");
		box->Position() = Vec2(100.f + 70*i, 240.f);
		layer->AddChild(box);
		shadow->AddShadowCaster(box);
	}

	return app.MainLoop();
}
