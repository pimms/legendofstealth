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
 *
 * The UDP socket is not used to retrieve data. All data is retrieved on the
 * server's incoming UDP socket. The UDP socket is only used to send data to
 * the client.
 *
 * TCP data is sent and retrieved on the TCP socket.
 */
class RemotePlayer {
public:
	RemotePlayer(unsigned playerID, Socket *tcpSocket);
	~RemotePlayer();

	unsigned GetPlayerID();

	bool IsConnected();

	void Update();

	void Disconnect();

private:
	unsigned _playerID;
	Socket *_tcp;
	Socket *_udp;
	RemotePlayerState _state;

	void SendJoinRequest();

	// Negotiation Update
	void HandleJoinRequest();
	void SendJoinResponse(bool response, Team team);
};

