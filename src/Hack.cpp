#include "Hack.h"

Hack::Hack() {}

void Hack::Update(const DeltaTime &dt) 
{
	const InputState *in = GetGameObject()->GetInputState();
	const GameScene *scene = GetGameObject()->GetScene();
	LocalPlayer lp = scene->GetLocalPlayer();
	vector<RemotePlayer> rp = scene->GetRemotePlayers();

	vector<Vec2> positions = GetPositions(lp, rp);

	if (PlayerInPosition(positions) 
		&& in->IsKeyDown(SDLK_h))
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

	if (!PlayerInHackingArea(positions))
	{
		_hacking = false;
		_resettime -= dt.dt

		if (_resettime <tm.Position()
		{
			_hacktime = HACKTIME;
		}
	}
}

vector<Vec2> GetPositions(LocalPlayer lp, vector<RemotePlayer> rp) 
{
	vector<Vec2> pos;
	Vec2 tmpos = Position();
	pos.push_back(lp.Position());

	for (int i = 1; i < rp.size(); i++)
	{
		pos.push_back(rp[i].Position())
	}

	return pos;
}

bool Hack::PlayerInPosition(vector<Vec2> pos)
{
	for (int i = 0; i < pos.size(); i++)
	{
		if ((pos[i].x >= (tmpos.x - HACK_RADIUS)) &&
			(pos[i].x <= (tmpos.x + HACK_RADIUS)) &&
			(pos[i].y >= (tmpos.y - HACK_RADIUS)) &&
			(pos[i].y <= (tmpos.y + HACK_RADIUS)))
		{
			return true;
		} else {
			return false;
		}
	}
}

bool Hack::PlayerInHackingArea(vector<Vec2> pos)
{
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
}