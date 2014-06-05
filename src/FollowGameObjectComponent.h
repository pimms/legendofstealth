#pragma once

#include <trutle/Trutle.h>


class FollowGameObjectComponent : public Component {
public:
	void SetTarget(GameObject *target, Vec2 offset);
	void Update(const DeltaTime &dt);

private:
	GameObject *_target;
	Vec2 _offset;
};
