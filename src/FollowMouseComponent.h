#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class FollowMouseComponent : public Component {
public:
	void Update(const DeltaTime &dt);
};
