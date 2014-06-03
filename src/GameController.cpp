#include "GameController.h"
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
GameController public methods
================
*/
GameController::GameController()
{
	
}

GameController::~GameController()
{
	if (_netcon)
		delete _netcon;
}

void GameController::LoadContent()
{
	Controller::LoadContent();

	_netcon = new NetworkController("localhost");

	Scene *scene = new Scene;
	SetScene(scene);

	Layer *layer = new Layer;
	scene->AddLayer(layer);

	GameObject *background = new GameObject;
	background->LoadTexture("res/bg.png");
	background->Position() = Vec2(320.f, 240.f);
	layer->AddChild(background);

	ShadowLayer *shadow = new ShadowLayer;
	scene->AddLayer(shadow);

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

void GameController::Update(DeltaTime &dt)
{
	Controller::Update(dt);
}

