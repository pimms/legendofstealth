#include "ConnectionListener.h"
#include <assert.h>
#include <chrono>



/*
================
Thread Functions
================
*/
static void TcpLoop(bool *abort, ConnectionListener *listener, 
					NewConnectionCallback cb)
{
	IPaddress ip;
	TCPsocket server_socket = 0;
	TCPsocket new_socket = 0;

	if (SDLNet_ResolveHost(&ip, NULL, TCP_SERVER_PORT) == -1) {
		string msg = (string)"SDLNet_ResolveHost() failed : " + SDLNet_GetError();
		Log::Error(msg);
		throw std::runtime_error(msg);
	}

	server_socket = SDLNet_TCP_Open(&ip);
	if (!server_socket) {
		string msg = (string)"SDLNet_TCP_OPEN() failed: " + SDLNet_GetError();
		Log::Error(msg);
		throw std::runtime_error(msg);
	}

	while (!(*abort)) {
		if (new_socket) {
			bool accept = (*listener.*cb)(new_socket);
			if (accept)
				new_socket = 0;
		} else {
			new_socket = SDLNet_TCP_Accept(server_socket);
			if (new_socket)
				Log::Debug("New TCP connection!");
		}

		
		// Sleep for 100 milliseconds
		std::this_thread::sleep_for(std::chrono::microseconds(100000));
	}
}


/*
================
ConnectionListener Public
================
*/
ConnectionListener::ConnectionListener()
	:	 _tcpsock(0),
		_abortThreads(false)
{
	_thread = new std::thread(TcpLoop, &_abortThreads, 
			this, &ConnectionListener::SetSocket);
}

ConnectionListener::~ConnectionListener()
{
	_abortThreads = true;
	_thread->join();
	delete _thread;
}

bool ConnectionListener::HasNewConnection() 
{
	_mutex.lock();

	bool result = false;
	result = (bool)_tcpsock;

	_mutex.unlock();

	return result;
}


Socket* ConnectionListener::GetSocket()
{
	_mutex.lock();

	TCPsocket sock = _tcpsock;
	_tcpsock = 0;

	_mutex.unlock();

	return new Socket(sock);
}

bool ConnectionListener::SetSocket(TCPsocket socket)
{
	bool success = false;

	_mutex.lock();

	if (!_tcpsock) {
		_tcpsock = (TCPsocket)socket;
		success = true;
	}

	_mutex.unlock();

	return success;
}
