#include "FollowGameObjectComponent.h"


void FollowGameObjectComponent::SetTarget(GameObject *target, Vec2 offset)
{
	_target = target;
	_offset = offset;
}

void FollowGameObjectComponent::Update(const DeltaTime &dt)
{
	if (_target) {
		Position().x = -_target->Position().x + _offset.x;
		Position().y = -_target->Position().y + _offset.y;
	}
}
