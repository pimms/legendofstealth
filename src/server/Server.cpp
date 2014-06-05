#include "Server.h"
#include <sstream>


Server::Server()
	:	_nextID(1),
		_udpSocket(new Socket(UDP, IPaddress{0, UDP_SERVER_PORT}, UDP_SERVER_PORT))
{
}

Server::~Server()
{
	for (int i=0; i<_remotePlayers.size(); i++) {
		delete _remotePlayers[i];
	}

	delete _udpSocket;
}


void Server::ServerLoop()
{
	while (true) {
		HandleNewConnection();
		HandleDisconnections();
		HandleIncomingUDP();
		BroadcastToPlayers();

		usleep(40000);
	}
}


/*
================
Server Private
================
*/
void Server::HandleNewConnection()
{
	if (_connectionListener.HasNewConnection()) {
		Socket *tcp = _connectionListener.GetSocket();
		RemotePlayer *rplayer = NULL;

		rplayer = new RemotePlayer(_nextID++, DecideTeamForNewPlayer(), tcp);

		for (int i=0; i<_remotePlayers.size(); i++) {
			// Notify the other players of the new dude
			PacketPlayerDidJoin pkt;
			pkt.type = PACKET_PLAYER_DID_JOIN;
			pkt.team = (unsigned)rplayer->GetTeam();
			pkt.playerID = rplayer->GetPlayerID();
			_remotePlayers[i]->SendPacket(TCP, &pkt);

			// Notify the new player of all the existing guys
			pkt.team = _remotePlayers[i]->GetTeam();
			pkt.playerID = _remotePlayers[i]->GetPlayerID();
			rplayer->SendPacket(TCP, &pkt);
		}

		_remotePlayers.push_back(rplayer);

		std::stringstream ss;
		ss << "Player joined: " << rplayer->GetPlayerID();
		Log::Debug(ss.str());
	}
}

Team Server::DecideTeamForNewPlayer()
{
	int spies = 0;
	int mercs = 0;
	GetTeamDistribution(spies, mercs);

	if (spies > mercs)
		return TEAM_MERC;
	if (mercs > spies)
		return TEAM_SPY;

	return TEAM_SPY;
}

void Server::GetTeamDistribution(int &spies, int &mercs)
{
	spies = 0;
	mercs = 0;

	for (int i=0; i<_remotePlayers.size(); i++) {
		RemotePlayer *rp = _remotePlayers[i];
		if (rp->GetTeam() == TEAM_SPY) {
			spies++;
		} else if (rp->GetTeam() == TEAM_MERC) {
			mercs++;
		}
	}
}


void Server::HandleDisconnections()
{
	for (int i=0; i<_remotePlayers.size(); i++) {
		_remotePlayers[i]->Update();
		if (!_remotePlayers[i]->IsConnected()) {
			std::stringstream ss;
			ss << "Player left: " << _remotePlayers[i]->GetPlayerID();
			Log::Debug(ss.str());

			delete _remotePlayers[i];
			_remotePlayers.erase(_remotePlayers.begin() + i--);
		}
	}
}


void Server::HandleIncomingUDP()
{
	while (_udpSocket->HasActivity()) {
		Packet *pkt = _udpSocket->GetPacket();

		if (pkt) {
			if (pkt->type == PACKET_PLAYER_UPDATE) {
				PacketPlayerUpdate *ppu = (PacketPlayerUpdate*)pkt;
				for (int i=0; i<_remotePlayers.size(); i++) {
					if (_remotePlayers[i]->GetPlayerID() == ppu->playerID) {
						_remotePlayers[i]->HandleUpdatePacket(ppu);
					}
				}
				delete pkt;
			} else {
				HandlePacket(pkt);		
			}
		}
	}
}

void Server::HandlePacket(Packet *packet)
{
	switch (packet->type) {
		case PACKET_PLAYER_FIRE:
			{
				PacketPlayerFire *pkt = (PacketPlayerFire*)packet;
				AddPacketToPlayerQueue(packet, pkt->playerID);
				printf("Added packet to queue\n");
				break;
			}

		case PACKET_PLAYER_HIT:
			{
				PacketPlayerHit *pkt = (PacketPlayerHit*)packet;
				AddPacketToPlayerQueue(packet, pkt->playerID);
				break;
			}

		default:
			Log::Warning("Received packet but was unable to handle it: " + PacketTypeStr(packet->type));
			delete packet;
			break;
	}
}

void Server::AddPacketToPlayerQueue(Packet *packet, unsigned playerID)
{
	for (int i=0; i<_remotePlayers.size(); i++) {
		if (_remotePlayers[i]->GetPlayerID() == playerID) {
			_remotePlayers[i]->AddPacketToQueue(packet);
			return;
		}
	}

	Log::Warning("Received packet of type " + PacketTypeStr(packet->type) + " from unkown player");
	delete packet;
}


void Server::BroadcastToPlayers()
{
	for (int i=0; i<_remotePlayers.size(); i++) {
		vector<Packet*> queue = _remotePlayers[i]->GetPacketQueue();

		for (int j=0; j<_remotePlayers.size(); j++) {
			if (j == i) continue;
			
			PacketPlayerUpdate pkt = _remotePlayers[i]->CreateUpdatePacket();
			_remotePlayers[j]->SendPacket(UDP, &pkt);			
			
			for (int k=0; k<queue.size(); k++) {
				_remotePlayers[j]->SendPacket(UDP, queue[k]);
			}
		}

		_remotePlayers[i]->ClearPacketQueue();
	}
}
