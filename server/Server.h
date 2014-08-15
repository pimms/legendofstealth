#pragma once

#include <SDL2/SDL_net.h>
#include <trutle/Trutle.h>
#include "ConnectionListener.h"
#include "Protocol.h"



namespace server {

class Client;

const int MAX_PLAYERS = 4;

class Server {
public:
	Server();
	~Server();

	void ServerLoop();

private:
	ConnectionListener _connectionListener;
	Socket *_udpSocket;
	ProtocolPhase _protoPhase;
	vector<Client*> _clients;


	void Broadcast(TLProtocol proto, Packet *packet);
	void FlushClientPacketQueues();

	void HandleNewTCPConnections();

	void AcceptNewClient(Socket *clientTcpSocket);
	bool IsServerFull() const;

	void TeamCounts(int &mercCount, int &spyCount) const;
	unsigned GetNextPlayerID() const;
	Team GetTeamForNextPlayer() const;

	/* - Adds a PACKET_PLAYER_DID_LEAVE to the queue of all other clients
	 * - Removes the client from the _clients-vector
	 */
	void KickClient(Client *client);
};

}
