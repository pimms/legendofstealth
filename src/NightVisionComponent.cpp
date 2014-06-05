#include "NightVisionComponent.h"
#include "GameScene.h"
#include "ShadowLayer.h"


void NightVisionComponent::Update(const DeltaTime &dt)
{
	const InputState *in = GetGameObject()->GetInputState();
	
	if (in->IsKeyFresh(SDLK_f)) {
		_enabled = !_enabled;
		GameScene *gs = GameScene::Singleton();
		
		Color color;
		if (_enabled) 
			color = Color(0.f, 0.25f, 0.f, 0.7f);
		else
			color = Color(0.f, 0.f, 0.f, 1.f);

		gs->GetShadowLayer()->SetShadowColor(color);
	}
}
