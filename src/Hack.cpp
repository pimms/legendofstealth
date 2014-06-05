#include "Hack.h"

/*
=========
Hackable
=========
*/
Hackable::Hackable() {}

void Hackable::Update(const DeltaTime &dt) 
{
	if (_hacking)
	{
		_hacktime -= dt.dt;
	}

	if (_hacktime <= 0) 
	{
		_hackdone = true;
		_hacking = false;
	}

	if (_hackinter)
	{
		_hacking = false;
	}
}


/*
============
Hacker
============
*/
Hacker::Hacker() {}

void Hacker::Update() {
	const InputState *in = GetGameObject()->GetInputState();
	GameScene *scene = (GameScene*)GetGameObject()->GetScene();
	LocalPlayer *lp = scene->GetLocalPlayer();
	vector<RemotePlayer*> rp = scene->GetRemotePlayers();

	vector<Vec2> positions = GetPositions(lp, rp);
	Vec2 tmpos = Position();
	printf("What the hell...?\n");

	if (PlayerInPosition(positions, tmpos) 
		&& in->IsKeyDown(SDLK_h))
	{
		StartHack();
	}

	if (!PlayerInHackingArea(positions, tmpos))
	{
		StopHack();
	}
}

vector<Vec2> Hacker::GetPositions(LocalPlayer *lp, vector<RemotePlayer*> rp) 
{
	vector<Vec2> pos;
	printf("Hey ho!\n");

	if (lp)
	{
		printf("Still no playa!\n");
		pos.push_back(lp->Position());
		printf("Yey, local playa!\n");
	}

	printf("Yolo McSwagger!\n");

	if (rp.size() <= 1)
	{
		printf("We've entered the lion's nest.\n");
		for (int i = 1; i < rp.size(); i++)
		{
			if (rp[i])
			{
				pos.push_back(rp[i]->Position());
				printf("RemotePlayer go: %i\n", i);
			}
		}
	}

	printf("We're outta here!\n");
	return pos;
}

bool Hacker::PlayerInPosition(vector<Vec2> pos, Vec2 tmpos)
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

bool Hacker::PlayerInHackingArea(vector<Vec2> pos, Vec2 tmpos)
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

void Hacker::StartHack()
{

}

void Hacker::StopHack() {

}