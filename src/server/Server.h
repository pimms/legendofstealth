#pragma once

#include <SDL2/SDL_net.h>
#include <trutle/Trutle.h>
#include "ConnectionListener.h"
#include "RemotePlayer.h"


class Server {
public:
	Server();
	~Server();

	void ServerLoop();

private:
	ConnectionListener _connectionListener;
	Socket *_udpSocket;
	vector<RemotePlayer*> _remotePlayers;

	unsigned _nextID;


	void HandleNewConnection();
	Team DecideTeamForNewPlayer();
	void GetTeamDistribution(int &spies, int &mercs);
	
	void HandleDisconnections();

	void HandleIncomingUDP();
	void HandlePacket(Packet *packet);
	void AddPacketToPlayerQueue(Packet *packet, unsigned playerID);

	void BroadcastToPlayers();
};
