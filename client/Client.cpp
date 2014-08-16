#include "Client.h"
#include "Protocol.h"

#include <ctime>
#include <unistd.h>


namespace client 
{


ServerConnector::ServerConnector(string host, unsigned tcpPort, unsigned udpPort) 
	:	_host(host),
		_tcpPort(tcpPort),
		_udpPort(udpPort),
		_tcpSocket(NULL),
		_udpSocket(NULL),
		_tcpGet(false),
		_udpGet(false),
		_isConnected(false),
		_playerID(0),
		_team(TEAM_MERC)
{

}

ServerConnector::~ServerConnector()
{
	// Delete the sockets if they were never retrieved.
	if (!_tcpGet && _tcpSocket) 
		delete _tcpSocket;
	if (!_udpGet && _udpSocket) 
		delete _udpSocket;
}


bool ServerConnector::ConnectToServer()
{
	// Already connected
	if (_isConnected) 
		return true;

	Log::Debug("Connecting to server %s:%u...", _host.c_str(), _tcpPort);

	_udpSocket = new Socket(UDP, _host, _udpPort);
	_tcpSocket = new SocketTCP(_host, _tcpPort);
	if (!_tcpSocket->IsConnectionOpen()) {
		Log::Error("Unable to connect to host %s:%u", 
				_host.c_str(), _tcpPort);
		CleanUpSockets();
		return false;
	}

	// Send a PACKET_JOIN_REQUEST 
	JoinRequestPacket pkt;
	pkt.port = _udpSocket->GetListenPortUDP();
	if (!_tcpSocket->SendPacket(&pkt)) {
		Log::Error("Unable to send PACKET_JOIN_REQUEST packet to %s:%u", 
				_host.c_str(), _tcpPort);
		CleanUpSockets();
		return false;
	}

	// Wait up to 10 seconds for a resopnse.
	// The only accepted response to a PACKET_JOIN_REQUEST
	// is a PACKET_JOIN_RESPONSE. If anything else is sent from
	// the server, ignore it's contents and log a warning. 
	std::time_t start = time(0);
	while (time(0) - start < 10) {
		while (_tcpSocket->HasActivity()) {
			Packet *pkt = _tcpSocket->GetPacket();
			if (pkt->GetPacketType() == PACKET_JOIN_RESPONSE) {
				JoinResponsePacket *respkt = (JoinResponsePacket*)pkt;
				
				if (!respkt->response) {
					Log::Error("Unable to connect to server: server rejected");
					CleanUpSockets();
					delete respkt;
					return false;
				} else {
					delete pkt;
					_playerID = respkt->playerID;
					_team = (Team)respkt->team;
					_isConnected = true;

					Log::Debug("Connected to server %s:%u",
							_host.c_str(), _tcpPort);
					Log::Verbose("PlayerID: %i", _playerID);
					Log::Verbose("Team: %s", (_team == TEAM_SPY ? "Spy" : "Merc"));
					return true;
				}
			} else {
				Log::Warning("Receved unexpected packet %s (expected PACKET_JOIN_RESPONSE)", 
						PacketTypeStr(pkt->GetPacketType()).c_str());
				delete pkt;
			}
		}

		// Sleep for 10ms
		usleep(10 * 1000);	
	}
	
	Log::Error("Failed to connect to server: connection timed out");
	CleanUpSockets();
	return false;
}

bool ServerConnector::IsConnectedToServer() const
{
	return _isConnected;
}


Socket* ServerConnector::GetTCPSocket() const
{
	AssertIsConnected(__PRETTY_FUNCTION__);
	return _tcpSocket;
}

Socket* ServerConnector::GetUDPSocket() const
{
	AssertIsConnected(__PRETTY_FUNCTION__);
	return _udpSocket;
}

unsigned ServerConnector::GetPlayerID() const
{
	AssertIsConnected(__PRETTY_FUNCTION__);
	return _playerID;
}

Team ServerConnector::GetTeam() const
{
	AssertIsConnected(__PRETTY_FUNCTION__);
	return _team;
}


void ServerConnector::AssertIsConnected(string context)  const
{
	if (!_isConnected) {
		throw std::runtime_error(
				context + ": ServerConnector is not connected");
	}
}

void ServerConnector::CleanUpSockets()
{
	if (_tcpSocket)
		delete _tcpSocket;
	if (_udpSocket)
		delete _udpSocket;

	_tcpSocket = NULL;
	_udpSocket = NULL;
}

}
