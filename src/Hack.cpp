#include "Hack.h"

Hack::Hack() {}

void Hack::Update(const DeltaTime &dt) 
{
	const InputState *in = GetGameObject()->GetInputState();

	if (playerInArea && in->IsKeyDown(SDLK_h))
	{
		_hacking = true;
	}

	if (_hacking)
	{
		_hacktime -= dt.dt;
	}

	if (_hacktime <= 0) 
	{
		_hackdone = true;
		_hacking = false;
	}

	if (!playerInHackingArea)
	{
		_hacking = false;
		_resettime -= dt.dt

		if (_resettime <= 0)
		{
			_hacktime = HACKTIME;
		}
	}
}