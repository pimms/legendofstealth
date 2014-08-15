#pragma once

#include <thread>
#include <mutex>

#include "Protocol.h"
#include "Socket.h"


/* Connection Listener
 *
 * Runs a background thread listening for new connections over TCP. The 
 * TCP socket is bound on port TCP_SERVER_PORT (can not currently be changed)
 */
class ConnectionListener {
public:
	ConnectionListener();
	~ConnectionListener();

	void AddNewSocket(TCPsocket);
	bool HasNewConnection();
	Socket* GetNewSocket();

private:
	std::thread *_thread;
	std::mutex _mutex;

	bool _abortThreads;

	vector<TCPsocket> _newConnections;
};

typedef void(ConnectionListener::*NewConnectionCallback)(TCPsocket);
