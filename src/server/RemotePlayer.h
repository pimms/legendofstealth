#pragma once

#ifdef _WIN32
#include <Trutle.h>
#include <SDL_net.h>
#else
#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>
#endif
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
	RemotePlayer(unsigned playerID, Team team, Socket *tcpSocket);
	~RemotePlayer();

	void Update();
	void Disconnect();

	void SendPacket(Protocol protocol, Packet *packet);
	PacketPlayerUpdate CreateUpdatePacket();
	void HandleUpdatePacket(PacketPlayerUpdate *packet);

	void AddPacketToQueue(Packet *packet);
	vector<Packet*> GetPacketQueue();
	void ClearPacketQueue();

	unsigned GetPlayerID();
	Team GetTeam();
	bool IsConnected();

private:
	unsigned _playerID;
	Socket *_tcp;
	Socket *_udp;
	RemotePlayerState _state;
	Team _team;

	// Always keeps the data from the last PacketPlayerUpdate
	unsigned _counter;
	Vec2 _position;
	float _rotation;

	// Store retrieved packets containing "one-time" events, basically
	// all packets bar PacketPlayerUpdate
	vector<Packet*> _recvPackets;


	void SendJoinRequest();

	// Negotiation Update
	void HandleJoinRequest();
	void SendJoinResponse(bool response);

	// Playing Update
	void HandleIncomingPackets();
};

