#pragma once

#include <trutle/Trutle.h>


/* The packet type is encoded in the header of each packet.
 * The remaining contents is dependent on the type of packet.
 *
 * All packets regarding the game-state is received by the server
 * and is broadcasted to all other players
 */
enum PacketType {
	// Initial message from client to server
	PACKET_JOIN_REQUEST,

	// Response to joining client from server
	PACKET_JOIN_RESPONSE,

	// Broadcasted to all players upon player join
	PACKET_SERVER_PLAYER_JOINED,

	// Broadcasted to all players upon player depart
	PACKET_SERVER_PLAYER_LEFT,

	// Sent from client to update it's position
	PACKET_PLAYER_UPDATE,

	// Sent from client upon firing
	PACKET_PLAYER_FIRE,

	// Sent from a client upon being hit by a bullet
	PACKET_PLAYER_HIT,

	// Sent from client upon beginning or ending a hack
	PACKET_PLAYER_HACK,
};


/* The "type" member indicates which sub-struct the object can be casted to.
 */
struct Packet {
	PacketType type;
};

struct PacketJoinRequest : public Packet {
	unsigned port;
};

struct PacketJoinResponse : public Packet {
	bool response;
	unsigned playerID;
};

struct PacketPlayerUpdate : public Packet {
	unsigned counter;
	unsigned playerID;
	float posX;
	float posY;
	float rotation;
};

struct PacketPlayerFire : public Packet {
	unsigned playerID;
};

struct PacketPlayerHit : public Packet {
	unsigned playerID;
};

struct PacketPlayerHack : public Packet {
	unsigned playerID;
	bool isHacking;
};

