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

static Vec2 _hackablepos;
static bool _hacking = false;
static bool _hackdone = false;
static bool _hackinter = false;
static float _hacktime = HACKTIME;
static float _resettime = RESET_TIME;
static float _showhackdone = 5;
static int _playerID;

class Hackable : public Component {
	public:
		Hackable();

		void Update(const DeltaTime &dt);
		

	private:
		Vec2 _position;
		GameScene *scene;

		void CheckProgress();
};

class Hacker : public Component {
	public:
		Hacker();

		void Update(const DeltaTime &dt);

		void SetUDPSocket(Socket *udp);
		void SendHackPacket();
		void SendHackPacketComplete();
	private:
		const float HACK_RADIUS = 50;
		const float AREA_RADIUS = 200;

		Socket *_udpSocket;


		vector<Vec2> GetPositions(vector<RemotePlayer*> rp);
		bool PlayerInPosition(Vec2 tmpos);
		bool PlayerInHackingArea(Vec2 tmpos);
		
		
};
