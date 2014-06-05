#pragma once

#include <trutle/Trutle.h>


class NightVisionComponent : public Component {
public:
	void Update(const DeltaTime &dt);

private:
	bool _enabled;
};
