#include "Hack.h"

/*
=========
Hackable
=========
*/
Hackable::Hackable() {}

void Hackable::Update(const DeltaTime &dt) 
{
	_hackablepos = Position();

	printf("Position of TERMINAL: %f, %f\n", _position.x, _position.y);

	if (_hacking)
	{
		_hacktime -= dt.dt;
		printf("Hacking remaining: %f\n", _hacktime);
	}

	if (_hacktime <= 0) 
	{
		_hackdone = true;
		_hacking = false;
	}

	if (_hackinter)
	{
		printf("Time until terminal RESET: %f\n", _resettime);
		_hacking = false;
		_resettime -= dt.dt;
	}

	if (_resettime <= 0)
	{
		_hackinter = false;
		_resettime = RESET_TIME;
		_hacktime = HACKTIME;
	}
}

/*
============
Hacker
============
*/
Hacker::Hacker() {}

void Hacker::Update(const DeltaTime &dt) {
	const InputState *in = GetGameObject()->GetInputState();
	GameScene *scene = (GameScene*)GetGameObject()->GetScene();
	vector<RemotePlayer*> rp = scene->GetRemotePlayers();

	printf("Position of SPY: %f, %f\n", Position().x, Position().y);

	//vector<Vec2> positions = GetPositions(rp);
	Vec2 tmpos = _hackablepos;
	printf("Position of TERMINAL: %f, %f\n", tmpos.x, tmpos.y);

	if (PlayerInPosition(_hackablepos))
	{
		printf("Start hacking NOW!\n");
	}

	if (PlayerInPosition(_hackablepos) 
		&& in->IsKeyDown(SDLK_h) && !_hackdone)
	{
		_hacking = true;
	}

	if (!PlayerInHackingArea(_hackablepos) && _hacking && !_hackdone)
	{
		_hackinter = true;
	} else if (PlayerInHackingArea(_hackablepos) && !_hacking && !_hackdone && _hackinter) {
		_hackinter = false;
		_resettime = RESET_TIME;
		_hacking = true;
	}
}

vector<Vec2> Hacker::GetPositions(vector<RemotePlayer*> rp) 
{
	vector<Vec2> pos;
	pos.push_back(Position());

	for (int i = 1; i < rp.size(); i++)
	{
		if (rp[i])
		{
			pos.push_back(rp[i]->Position());
		}
	}

	return pos;
}

bool Hacker::PlayerInPosition(Vec2 tmpos)
{
	printf("HACK_RADIUS: %f\n", HACK_RADIUS);
	printf("Position of Player: %f, %f\n", tmpos.x, tmpos.y);
	if ((Position().x >= (tmpos.x - HACK_RADIUS)) &&
		(Position().x <= (tmpos.x + HACK_RADIUS)) &&
		(Position().y >= (tmpos.y - HACK_RADIUS)) &&
		(Position().y <= (tmpos.y + HACK_RADIUS)))
	{
		return true;
	} else {
		return false;
	}

}

bool Hacker::PlayerInHackingArea(Vec2 tmpos)
{
	/*
	for (int i = 0; i < pos.size(); i++)
	{
		if ((pos[i].x >= (tmpos.x - AREA_RADIUS)) &&
			(pos[i].x <= (tmpos.x + AREA_RADIUS)) &&
			(pos[i].y >= (tmpos.y - AREA_RADIUS)) &&
			(pos[i].y <= (tmpos.y + AREA_RADIUS)))
		{
			return true;
		} else {
			return false;
		}
	}
	*/

	if ((Position().x >= (tmpos.x - AREA_RADIUS)) &&
		(Position().x <= (tmpos.x + AREA_RADIUS)) &&
		(Position().y >= (tmpos.y - AREA_RADIUS)) &&
		(Position().y <= (tmpos.y + AREA_RADIUS)))
	{
		return true;
	} else {
		printf("SPY has left the hacking area!\n");
		return false;
	}
}