#include "Server.h"
#include <sstream>


Server::Server()
	:	 _udpSocket(new Socket(UDP, IPaddress{0, UDP_SERVER_PORT}, UDP_SERVER_PORT)),
		_protoPhase(PROTO_LOBBY)
{
}

Server::~Server()
{
	delete _udpSocket;
}


void Server::ServerLoop()
{
	while (true) {
		if (_protoPhase == PROTO_NONE) {
			Log::Error("Internal server error. Shutting down...");
			
			// TODO:
			// Notify the clients of the server's departure

			break;
		}
	
		HandleNewTCPConnections();

		usleep(40000);
	}
}


/*
================
Server Private
================
*/
void Server::HandleNewTCPConnections() 
{
	while (_connectionListener.HasNewConnection()) {
		Socket *socket = _connectionListener.GetNewSocket();
		
		if (_protoPhase == PROTO_LOBBY) {
			// Accept the new player
		} else {
			// Reject the player
			JoinResponsePacket pkt;
			pkt.response = false;
			socket->SendPacket(&pkt);
		}
	}
}
