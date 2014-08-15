#include "Server.h"
#include "Client.h"
#include <sstream>


namespace server {

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
			
			ServerShutdownPacket pkt;
			Broadcast(&pkt);
			break;
		}
	
		HandleNewTCPConnections();

		FlushClientPacketQueues();

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
		
		if (_protoPhase == PROTO_LOBBY && !IsServerFull()) {
			AcceptNewClient(socket);
		} else {
			// Reject the player
			JoinResponsePacket pkt;
			pkt.response = false;
			socket->SendPacket(&pkt);
			delete socket;
		}
	}
}


void Server::Broadcast(TLProtocol proto, Packet *packet)
{
	for (int i=0; i<_clients.size(); i++) {
		Client *client = _clients[i];
		
		if (client->IsConnected()) {
			client->AddPacketToQueue(proto, packet);
		} else {
			KickClient(client);
			i--;
		}
	}
}

void Server::FlushClientPacketQueues() 
{
	for (int i=0; i<_clients.size(); i++) {
		Client *client = _clients[i];

		if (!client->IsConnected()) {
			Log::Error("Client %i disconnected", client->GetPlayerID());
			KickClient(client);
			i--;
		} else {
			if (!client->SendPacketQueue(TCP) || !client->SendPacketQueue(UDP)) {
				Log::Error("Failed to send packet queue to client %i", client->GetPlayerID());
				KickClient(client);
				i--;
			}
		}	
	} 
}


void Server::AcceptNewClient(Socket *clientTcpSocket)
{
	if (!clientTcpSocket->HasActivity()) {
		Log::Error("Client kicked before he even connected. No activity on socket.");
		delete clientTcpSocket;
		return;
	}

	Packet *packet = clientTcpSocket->GetPacket();
	if (!packet) {
		Log::Error("Unable to retrieve packet from newly connected socket");
		delete packet;
		delete clientTcpSocket;
		return;
	}

	if (packet->GetPacketType() != PACKET_JOIN_REQUEST) {
		Log::Error("PROTOCOL MISMATCH: Expected PACKET_JOIN_REQUEST, received %s", 
				PacketTypeStr(packet->GetPacketType()).c_str());
		delete packet;
		delete clientTcpSocket;
		return;
	}

	JoinRequestPacket *jrp = (JoinRequestPacket*)packet;
	unsigned udpPort = jrp->port;
	delete jrp;

	Client *client = new Client(GetNextPlayerID(), 
								GetTeamForNextPlayer(), 
								udpPort,
								clientTcpSocket);
	if (!client->SendJoinResponsePacket()) {
		delete client;
	} else {
		PlayerDidJoinPacket pkt;
		pkt.playerID = client->GetPlayerID();
		pkt.team = client->GetTeam();
		Broadcast(TCP, &pkt);

		_clients.push_back(client);
	}
}

bool Server::IsServerFull() const
{
	return _clients.size() >= MAX_PLAYERS;
}


void Server::TeamCounts(int &mercCount, int &spyCount) const
{
	mercCount = 0;
	spyCount = 0;

	for (int i=0; i<_clients.size(); i++) {
		Client *client = _clients[i];

		if (client->GetTeam() == TEAM_SPY) {
			spyCount++;
		} else if (client->GetTeam() == TEAM_MERC) {
			mercCount++;
		}
	}
}

unsigned Server::GetNextPlayerID() const
{
	static unsigned playerId = 1;
	return playerId++;
}

Team Server::GetTeamForNextPlayer() const
{
	int m = 0;
	int s = 0;
	TeamCounts(m, s);

	if (m > s) {
		return TEAM_SPY;
	} else {
		return TEAM_MERC;
	}
}


void Server::KickClient(Client *client)
{
	if (!client) {
		Log::Warning("Called Server::KickClient() with NULL");
		return;
	}
	
	// Add a leave packet to all the other clients
	PlayerDidLeavePacket leavePkt;
	leavePkt.playerID = client->GetPlayerID();

	int kickIndex = -1;

	for (int i=0; i<_clients.size(); i++) {
		if (_clients[i] == client) {
			kickIndex = i;
		} else {
			_clients[i]->AddPacketToQueue(TCP, &leavePkt);
		}
	}

	// Remove the kicked player
	delete client;

	if (kickIndex >= 0) {
		_clients.erase(_clients.begin() + kickIndex);
	}
}

}

