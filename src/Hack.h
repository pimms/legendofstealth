#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "GameScene.h"
#include "Player.h"

#define HACKTIME 100;
#define RESET_TIME 10;
#define HACK_RADIUS 50;
#define AREA_RADIUS 200;

class Hack : public Component {
	public:
		Hack();

		void Update(const DeltaTime &dt);

	private:
		bool _hacking = false, _hackdone = false;
		float _hacktime = HACKTIME;
		float _resettime = RESET_TIME;

		vector<Vec2> GetPositions(LocalPlayer lp, vector<RemotePlayer> rp);
		bool PlayerInPosition(vector<Vec2> pos);
		bool PlayerInHackingArea(vector<Vec2> pos);
};