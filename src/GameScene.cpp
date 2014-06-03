#include "GameScene.h"
#include "ShadowLayer.h"
#include "LightSource.h"
#include "ShadowCaster.h"

// Temporary component to just move an object around
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


/*
================
GameScene Public
================
 */
GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}


void GameScene::LoadContent()
{
	Scene::LoadContent();

	Layer *layer = new Layer;
	AddLayer(layer);

	GameObject *background = new GameObject;
	background->LoadTexture("res/bg.png");
	background->Position() = Vec2(320.f, 240.f);
	layer->AddChild(background);

	ShadowLayer *shadow = new ShadowLayer;
	AddLayer(shadow);

	// Create a couple of lights
	for (int i=0; i<4; i++) {
		LightSource *light = new LightSource;
		light->SetLightDistance(150.f);
		light->LoadTexture("res/light.png");
		light->Position() = Vec2(100.f + 120*i, 200.f);
		light->Pivot() = Vec2(0.5f, 0.5f);
		layer->AddChild(light);
		shadow->AddLightSource(light);
		if (!i) {
			AddComponent<MoveComponent>(light);
		} else {
			if (rand() % 2 == 0) {
				Color c(1.f, 0.f, 0.f, 1.f);
				light->SetColor(c);
			} else {
				Color c(0.f, 0.f, 1.f, 1.f);
				light->SetColor(c);
			}
		}
	}

	// Create a couple of boxes
	for (int i=0; i<5; i++) {
		ShadowCaster *box = new ShadowCaster;
		box->LoadTexture("res/box.png");
		box->Position() = Vec2(100.f + 70*i, 240.f);
		layer->AddChild(box);
		shadow->AddShadowCaster(box);
	}
}

void GameScene::Update(const DeltaTime &dt)
{
	Scene::Update(dt);
}
