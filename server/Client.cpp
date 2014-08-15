#include "Client.h"
#include "Protocol.h"


namespace server {


/*
================ 
Client Public
================ 
*/
Client::Client(unsigned playerID, Team team, unsigned udpPort, Socket *tcpSocket)
	:	_playerID(playerID),
		_team(team),
		_tcpSocket(tcpSocket),
		_udpPort(udpPort)
{
	IPaddress ipa;
	ipa.port = _udpPort;
	ipa.host = _tcpSocket->GetIPaddress().host;
	_udpSocket = new Socket(UDP, ipa, 0);
}

Client::~Client()
{
	delete _tcpSocket;
	delete _udpSocket;
}


void Client::AddPacketToQueue(TLProtocol proto, Packet *packet)
{
	int len = 0;
	byte *buf = packet->Serialize(len);

	vector<byte> *store = (proto == UDP) ? &_udpPktQueue : &_tcpPktQueue;

	for (int i=0; i<len; i++) {
		store->push_back(buf[i]);
	}
}

bool Client::SendPacketQueue(TLProtocol proto)
{
	Socket *socket = (proto == UDP) ? _udpSocket : _tcpSocket;
	vector<byte> *queue = (proto == UDP) ? &_udpPktQueue : &_tcpPktQueue;

	if (queue->size() == 0) {
		return true;
	}

	bool result = socket->SendPacket(&(*queue)[0], queue->size());
	queue->clear();

	return result;
}


bool Client::SendPacket(Packet *packet)
{
	bool result = _tcpSocket->SendPacket(packet);

	if (!result) {
		Log::Error("Unable to send packet of type %s to client %i",
				PacketTypeStr(packet->GetPacketType()).c_str(), _playerID);
	}

	return result;
}

bool Client::SendJoinResponsePacket()
{
	JoinResponsePacket pkt;
	pkt.response = true;
	pkt.playerID = _playerID;
	pkt.team = _team;

	return SendPacket(&pkt);
}


bool Client::IsConnected()
{
	return _tcpSocket->IsConnectionOpen();
}

unsigned Client::GetPlayerID() const
{
	return _playerID;
}

Team Client::GetTeam() const
{
	return _team;
}

unsigned Client::GetUdpPort() const
{
	return _udpPort;
}


/*
================ 
Client Private 
================ 
*/


}
