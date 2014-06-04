#include "RemotePlayer.h"


/*
================
RemotePlayer Public
================
*/
RemotePlayer::RemotePlayer(unsigned playerID, Socket *tcpSocket)
	:	_tcp(tcpSocket),
		_udp(NULL),
		_playerID(playerID)
{
}

RemotePlayer::~RemotePlayer()
{
	if (_tcp)
		delete _tcp;
	if (_udp)
		delete _udp;
}


unsigned RemotePlayer::GetPlayerID()
{
	return _playerID;
}


bool RemotePlayer::IsConnected()
{
	return !_tcp->IsConnectionOpen();
}



/*
================
RemotePlayer Private 
================
*/

