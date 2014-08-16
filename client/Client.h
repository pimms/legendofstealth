#pragma once

#include <trutle/Trutle.h>
#include "Protocol.h"
#include "Socket.h"


namespace client {


/* ServerConnector
 * Class responsible for establishing a connection to the
 * server and joining a lobby. 
 *
 * A successful call to ConnectToServer() will ensure that
 * GetTCPSocket(), GetUDPSocket(), GetPlayerID() and GetTeam()
 * all return valid values. The Socket instances returned by
 * GetXxxSocket() are the responsibility of the caller and will
 * not be deleted by the ServerConnector (unless the getters are
 * never called, then the Sockets are deleted in the destructor).
 */
class ServerConnector {
public:
	ServerConnector(string host, 
					unsigned tcpPort=TCP_SERVER_PORT, 
					unsigned udpPort=UDP_SERVER_PORT);
	~ServerConnector();

	bool ConnectToServer();
	bool IsConnectedToServer() const;

	Socket* GetTCPSocket() const;
	Socket* GetUDPSocket() const;
	unsigned GetPlayerID() const;
	Team GetTeam() const;

private:
	string _host;
	unsigned _tcpPort;
	unsigned _udpPort;

	Socket *_tcpSocket;
	Socket *_udpSocket;

	bool _tcpGet;
	bool _udpGet;
	bool _isConnected;

	unsigned _playerID;
	Team _team;

	void AssertIsConnected(string context) const;
	void CleanUpSockets();
};


}
