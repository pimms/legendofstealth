#pragma once

#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>
#include "../net/Socket.h"

enum RemotePlayerState {
	// The player has not connected or has disconnected
	STATE_DC,

	// The player is connected but does not have an ID
	STATE_NEGOTIATE,

	// The player is playing and everything is k
	STATE_PLAYING,
};

/* Remote Player
 *
 * Partly a FSM.
 */
class RemotePlayer {
public:
	RemotePlayer(unsigned playerID, Socket *tcpSocket);
	~RemotePlayer();

	unsigned GetPlayerID();

	bool IsConnected();

private:
	unsigned _playerID;
	Socket *_tcp;
	Socket *_udp;
};

