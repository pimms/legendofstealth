#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "GameScene.h"
#include "Player.h"

const float HACKTIME = 100;
const float RESET_TIME = 10;

Vec2 _hackablepos;
bool _hacking = false;
bool _hackdone = false;
bool _hackinter = false;
float _hacktime = HACKTIME;
float _resettime = RESET_TIME;

class Hackable : public Component {
	public:
		Hackable();

		void Update(const DeltaTime &dt);
		void StartHack();
		void HackInterrupt();

		bool GetCurHacking();
		bool HackIsFinished();
		Vec2 GetTmPosition();

	private:
		Vec2 _position;
		
		void PrintInfo();
};

class Hacker : public Component {
	public:
		Hacker();

		void Update(const DeltaTime &dt);
	private:
		const float HACK_RADIUS = 50;
		const float AREA_RADIUS = 200;

		vector<Vec2> GetPositions(vector<RemotePlayer*> rp);
		bool PlayerInPosition(Vec2 tmpos);
		bool PlayerInHackingArea(Vec2 tmpos);
};