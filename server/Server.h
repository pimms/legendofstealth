#pragma once

#include <SDL2/SDL_net.h>
#include <trutle/Trutle.h>
#include "ConnectionListener.h"
#include "Protocol.h"


class Server {
public:
	Server();
	~Server();

	void ServerLoop();

private:
	ConnectionListener _connectionListener;
	Socket *_udpSocket;

	ProtocolPhase _protoPhase;


	void HandleNewTCPConnections();
};
