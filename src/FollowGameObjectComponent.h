#pragma once


#ifdef _WIN32
#include <Trutle.h>
#else
#include <trutle/Trutle.h>
#endif


class FollowGameObjectComponent : public Component {
public:
	void SetTarget(GameObject *target, Vec2 offset);
	void Update(const DeltaTime &dt);

private:
	GameObject *_target;
	Vec2 _offset;
};
