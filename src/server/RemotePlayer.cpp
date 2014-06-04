#include "RemotePlayer.h"


/*
================
RemotePlayer Public
================
*/
RemotePlayer::RemotePlayer(unsigned playerID, Team team, Socket *tcpSocket)
	:	_tcp(tcpSocket),
		_udp(NULL),
		_playerID(playerID),
		_state(STATE_NEGOTIATE),
		_rotation(0.f),
		_counter(0),
		_team(team)
{

}

RemotePlayer::~RemotePlayer()
{
	if (_tcp)
		delete _tcp;
	if (_udp)
		delete _udp;
}


void RemotePlayer::Update()
{
	switch (_state) {
		case STATE_DC:
			break;

		case STATE_NEGOTIATE:
			HandleJoinRequest();
			break;

		case STATE_PLAYING:
			HandleIncomingPackets();
			break;

		default:
			Log::Error("Unhandled state in RemotePlayer");
			return;
	}
}

void RemotePlayer::Disconnect()
{
	if (_tcp)
		delete _tcp;
	_tcp = NULL;

	if (_udp)
		delete _udp;
	_udp = NULL;

	_state = STATE_DC;
}


void RemotePlayer::SendPacket(Protocol protocol, Packet *packet)
{
	if (protocol == TCP && _tcp) {
		_tcp->SendPacket(packet);
	} else if (protocol == UDP && _udp) {
		_udp->SendPacket(packet);
	}
}

PacketPlayerUpdate RemotePlayer::CreateUpdatePacket()
{
	PacketPlayerUpdate pkt;
	pkt.type = PACKET_PLAYER_UPDATE;
	pkt.posX = _position.x;
	pkt.posY = _position.y;
	pkt.rotation = _rotation;
	pkt.playerID = _playerID;
	pkt.counter = 0;

	return pkt;
}

void RemotePlayer::HandleUpdatePacket(PacketPlayerUpdate *packet)
{
	_position.x = packet->posX;
	_position.y = packet->posY;
	_rotation = packet->rotation;
	_counter = packet->counter;
}


unsigned RemotePlayer::GetPlayerID()
{
	return _playerID;
}

Team RemotePlayer::GetTeam()
{
	return _team;
}


bool RemotePlayer::IsConnected()
{
	return _tcp->IsConnectionOpen();
}





/*
================
RemotePlayer Private 
================
*/
void RemotePlayer::HandleJoinRequest()
{
	if (_tcp->HasActivity()) {
		Packet *p = _tcp->GetPacket();

		if (p) {
			if (p->type != PACKET_JOIN_REQUEST) {
				Log::Error("Protocol mismatch: expected PACKET_JOIN_REQUEST");
				Disconnect();
				return;
			}

			PacketJoinRequest *pjr = (PacketJoinRequest *)p;
			unsigned udpPort = pjr->port;

			IPaddress ipa = _tcp->GetIPaddress();
			ipa.port = udpPort;
			_udp = new Socket(UDP, ipa, 0);

			// TODO
			// Broadcast the addition of the new player
			Log::Debug("Player joined the game");

			// TODO
			// Assign teams
			SendJoinResponse(true);
			delete p;

			_state = STATE_PLAYING;
		}
	}
}

void RemotePlayer::SendJoinResponse(bool response)
{
	PacketJoinResponse *pjr = new PacketJoinResponse();
	pjr->type = PACKET_JOIN_RESPONSE;
	pjr->response = true;
	pjr->playerID = _playerID;
	pjr->team = (unsigned)_team;
	_tcp->SendPacket(pjr);
	
	delete pjr;
}


void RemotePlayer::HandleIncomingPackets()
{
	while (_udp->HasActivity()) {
		Packet *pkt = _udp->GetPacket();
		if (pkt) {
			switch (pkt->type) {
				case PACKET_PLAYER_UPDATE:
					HandlePlayerUpdate((PacketPlayerUpdate*)pkt);
					break;

				default:
					Log::Warning("Unhandled UDP packet: " + PacketTypeStr(pkt->type));
					break;
			}
		} else {
			break;
		}
	}

	while (_tcp->HasActivity()) {
		Packet *pkt = _tcp->GetPacket();
		if (pkt) {
			switch (pkt->type) {
				default:
					Log::Warning("Unhandled TCP packet: " + PacketTypeStr(pkt->type));
					break;
			}
		} else {
			break;
		}
	}
}

void RemotePlayer::HandlePlayerUpdate(PacketPlayerUpdate *pkt)
{
	_position = Vec2(pkt->posX, pkt->posY);
	_rotation = pkt->rotation;
	_counter = pkt->counter;
}
