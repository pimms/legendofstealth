#include "FireComponent.h"
#include "Entity.h"

FireComponent::FireComponent(){	}


void FireComponent::Update(const DeltaTime &dt){
	const InputState *in = GetGameObject()->GetInputState();
	if (in->IsKeyDown(SDLK_e)){
		// Get position from player
		Entity* entity = (Entity*)GetGameObject();
		RayCastUtility callback;
		Vec2 pos1 (Position().x, Position().y);
		
		b2Vec2 p1 = Tob2Vec2(pos1);
		b2Vec2 p2 = calculateDirection();
		
		entity->getWorld()->RayCast(&callback, p1, p2);
		
		if (callback.m_fixture != 0){
			GameObject *go = (Entity*)callback.m_fixture->GetUserData();
			
			if (dynamic_cast<Player*>(go)) {
				printf("You hit something\n");
			}
		}

		if (callback.m_fixture == 0){
			printf("\nDidn't hit anything");
		}
		
		
	}

}


b2Vec2 FireComponent::calculateDirection()
{
	float radFireComp = atan2f(Position().y, Position().x);
	float degFireComp = Rad2Deg(radFireComp);

	Vec2 dir(cosf(degFireComp), sinf(degFireComp));
	dir.x *= FIRE_LENGTH;
	dir.y *= FIRE_LENGTH;
	return b2Vec2(Tob2Vec2(dir));
}
