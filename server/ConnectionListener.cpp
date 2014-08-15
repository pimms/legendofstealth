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
		TCPsocket new_socket = SDLNet_TCP_Accept(server_socket);

		if (new_socket) {
			Log::Debug("New TCP connection!");
			(*listener.*cb)(new_socket);
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
	:	 _abortThreads(false)
{
	_thread = new std::thread(TcpLoop, &_abortThreads, 
			this, &ConnectionListener::AddNewSocket);
}

ConnectionListener::~ConnectionListener()
{
	_abortThreads = true;
	_thread->join();
	delete _thread;
}


void ConnectionListener::AddNewSocket(TCPsocket socket)
{
	if (socket > 0) {
		_mutex.lock();
		_newConnections.push_back(socket);
		_mutex.unlock();
	}
}

bool ConnectionListener::HasNewConnection() 
{
	_mutex.lock();
	int count = _newConnections.size();
	_mutex.unlock();

	return (count > 0);
}

Socket* ConnectionListener::GetNewSocket()
{
	TCPsocket socket = 0;

	_mutex.lock();

	if (_newConnections.size() > 0) {
		socket = _newConnections[0];
		_newConnections.erase(_newConnections.begin());
	}

	_mutex.unlock();

	if (socket == 0) {
		return NULL;
	}

	return new Socket(socket);
}

