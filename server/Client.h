#pragma once

#include <trutle/Trutle.h>
#include "Protocol.h"
#include "Socket.h"



namespace server 
{

class Client {
public:
	Client(unsigned playerID, Team team, unsigned udpPort, Socket *tcpSocket);
	~Client();

	void AddPacketToQueue(TLProtocol proto, Packet *packet);

	// - Returns true if the queue is empty or on success
	// - Returns false on error
	bool SendPacketQueue(TLProtocol proto);

	bool SendPacket(Packet *pkt);
	bool SendJoinResponsePacket();

	bool IsConnected();
	unsigned GetPlayerID() const;
	Team GetTeam() const;
	unsigned GetUdpPort() const;

private:
	unsigned _playerID;
	Team _team;
	Socket *_tcpSocket;
	Socket *_udpSocket;
	unsigned _udpPort;

	vector<byte> _tcpPktQueue;
	vector<byte> _udpPktQueue;
};


}
