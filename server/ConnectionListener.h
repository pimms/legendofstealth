#pragma once

#include <thread>
#include <mutex>

#include "Packet.h"
#include "Socket.h"


/* Connection Listener
 *
 * Runs a background thread listening for new connections over TCP
 */
class ConnectionListener {
public:
	ConnectionListener();
	~ConnectionListener();

	bool HasNewConnection();

	Socket* GetSocket();

	bool SetSocket(TCPsocket);

private:
	TCPsocket _tcpsock;
	std::thread *_thread;
	std::mutex _mutex;

	bool _abortThreads;
};

typedef bool(ConnectionListener::*NewConnectionCallback)(TCPsocket);
