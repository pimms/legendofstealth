#pragma once

#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>
#include <thread>
#include <mutex>

#include "../net/Packet.h"
#include "../net/Socket.h"


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
