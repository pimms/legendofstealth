#include "Hack.h"
#include "net/Packet.h"
#include "net/Socket.h"

/*
=========
Hackable
=========
*/
Hackable::Hackable() {}

void Hackable::Update(const DeltaTime &dt) 
{
	scene = (GameScene*)GetGameObject()->GetScene();
	_hackablepos = Position();

	if (_hacking)
	{
		CheckProgress();
		_hacktime -= dt.dt;
		printf("Hacking remaining: %f\n", _hacktime);
	}

	if (_hacktime <= 0) 
	{
		//scene->LoadOverlay("redhackcomplete.png");
		//scene->RemoveOverlay("red.png");
		_hackdone = true;
		_hacking = false;
	}

	/*
	if (_hackdone)
	{
		_showhackdone -= dt.dt;
	}
	*/

	if (_showhackdone <= 0)
	{
		//scene->RemoveOverlay("redhackcomplete.png");
	}

	if (_hackinter)
	{
		printf("TERMINAL has been RESET\n");
		_hacking = false;
		_hackinter = false;
		_hacktime = HACKTIME;
	}
}

void Hackable::CheckProgress() {
	if ((HACKTIME / _hacktime) <= 3/3) {
		//scene->LoadOverlay("green.png");
	} else if ((HACKTIME / _hacktime) <= (2/3)) {
		//scene->LoadOverlay("yellow.png");
		//scene->RemoveOverlay("green.png");
	} else if ((HACKTIME / _hacktime) <= (1/3)) {
		//scene->LoadOverlay("red.png");
		//scene->RemoveOverlay("yellow.png");
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
	Vec2 tmpos = _hackablepos;

	if (PlayerInPosition(_hackablepos) && !_hacking)
	{
		printf("Start hacking NOW, by pressing 'E'!\n");
	}

	if (PlayerInPosition(_hackablepos) 
		&& in->IsKeyDown(SDLK_e) && !_hackdone)
	{
		if (_hacking == false)
		{
			SendHackPacket();
		}

		_hacking = true;
	}

	if (_hackdone)
	{
		SendHackPacketComplete();
	}

	if (!PlayerInHackingArea(_hackablepos) && _hacking && !_hackdone)
	{
		_hackinter = true;
		SendHackPacket();
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
	if ((Position().x >= (tmpos.x - AREA_RADIUS)) &&
		(Position().x <= (tmpos.x + AREA_RADIUS)) &&
		(Position().y >= (tmpos.y - AREA_RADIUS)) &&
		(Position().y <= (tmpos.y + AREA_RADIUS)))
	{
		return true;
	} else {
		return false;
	}
}

void Hacker::SendHackPacket() {
	PacketPlayerHack packet;

	packet.type = PACKET_PLAYER_HACK;
	packet.playerID = ((Player*)GetGameObject())->GetPlayerID();
	packet.terminalID = 1;
	packet.isHacking = _hacking;

	_udpSocket->SendPacket(&packet);
}

void Hacker::SetUDPSocket(Socket *udp)
{
	_udpSocket = udp;
}

void Hacker::SendHackPacketComplete() {
	PacketHackComplete packet;

	packet.type = PACKET_HACK_COMPLETE;
	packet.playerID = ((Player*)GetGameObject())->GetPlayerID();
	packet.terminalID = 1;

	_udpSocket->SendPacket(&packet);
}