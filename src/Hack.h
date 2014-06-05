#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "GameScene.h"
#include "Player.h"

class Hackable : public Component {
	public:
		Hackable();

		void Update(const DeltaTime &dt);
		void StartHack();
		void HackInterrupt();

		bool GetCurHacking();
		bool HackIsFinished();

	private:
		const float HACKTIME = 100;
		const float RESET_TIME = 10;
		

		bool _hacking = false;
		bool _hackdone = false;
		bool _hackinter = false;

		float _hacktime = HACKTIME;
		float _resettime = RESET_TIME;

		void PrintInfo();
};

class Hacker : public Component {
	public:
		Hacker();

		void Update();
	private:
		const float HACK_RADIUS = 50;
		const float AREA_RADIUS = 200;

		bool hacked = false;

		Hackable hackable;

		void StartHack();
		void StopHack();

		vector<Vec2> GetPositions(LocalPlayer *lp, vector<RemotePlayer*> rp);
		bool PlayerInPosition(vector<Vec2> pos, Vec2 tmpos);
		bool PlayerInHackingArea(vector<Vec2> pos, Vec2 tmpos);
};