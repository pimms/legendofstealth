#pragma once

#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>

#define TCP_SERVER_PORT			43720
#define UDP_SERVER_PORT			43720


enum Protocol {
	UDP,
	TCP,
};

typedef unsigned char byte;


/* The packet type is encoded in the header of each packet.
 * The remaining contents is dependent on the type of packet.
 *
 * All packets regarding the game-state is received by the server
 * and is broadcasted to all other players.
 *
 * The PacketType is encoded in transmitted packages as a single
 * 8 bit integral.
 */
enum PacketType {
	// Initial message from client to server
	PACKET_JOIN_REQUEST,

	// Response to joining client from server
	PACKET_JOIN_RESPONSE,

	// Broadcasted to all players upon player join
	PACKET_PLAYER_DID_JOIN,

	// Broadcasted to all players upon player depart
	PACKET_PLAYER_DID_LEAVE,

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
	static Packet* ReadPacket(byte *buffer, int bufferlen, int &packetlen);

	Packet();
	virtual ~Packet();
	virtual int FillPacketFromBuffer(byte *buffer, int len) = 0;
	virtual byte* GetSendablePacket(int &packetlen) = 0;

	PacketType type;

	/* Used ONLY when the Packet was RECEIVED over UDP */
	UDPpacket *udpPacket;

protected:
	static int ReadInt(byte *buf);
	static unsigned ReadUInt(byte *buf);
	static float ReadFloat(byte *buf);
};

struct PacketJoinRequest : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned port;
};

struct PacketJoinResponse : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);
	
	bool response;
	unsigned playerID;
	unsigned team;
};

struct PacketPlayerDidJoin : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned playerID;
	unsigned team;
};

struct PacketPlayerDidLeave : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned playerID;
};

struct PacketPlayerUpdate : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned counter;
	unsigned playerID;
	float posX;
	float posY;
	float rotation;
};

struct PacketPlayerFire : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned playerID;
};

struct PacketPlayerHit : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned playerID;
};

struct PacketPlayerHack : public Packet {
	int FillPacketFromBuffer(byte *buffer, int len);
	byte* GetSendablePacket(int &packetlen);

	unsigned playerID;
	bool isHacking;
};

