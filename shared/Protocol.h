#pragma once

#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>
#include <map>

#define TCP_SERVER_PORT			43720
#define UDP_SERVER_PORT			43720


enum TLProtocol {
	UDP,
	TCP,
};

enum Team {
	TEAM_SPY,
	TEAM_MERC,
};

enum ProtocolPhase {
	PROTO_NONE,
	PROTO_NEGOT,
	PROTO_LOBBY,
	PROTO_GAME,
};

typedef unsigned char byte;


enum PacketType {
	PACKET_UNDEFINED,
	
	// Initial message from client to server
	PACKET_JOIN_REQUEST,

	// Response to joining client from server
	PACKET_JOIN_RESPONSE,

	// Broadcasted to all players upon player join
	PACKET_PLAYER_DID_JOIN,

	// Broadcasted to all remaining players upon a player's departure
	PACKET_PLAYER_DID_LEAVE,

	// Broadcasted to all connected players upon server shutdown
	PACKET_SERVER_SHUTDOWN,
};

string PacketTypeStr(PacketType type);


class Packet;

class PacketFactory {
public:
	static int ReadInt(const byte *buf);
	static unsigned ReadUInt(const byte *buf);
	static float ReadFloat(const byte *buf);

	// The returned unsigned can be cast directly to byte[4] and be
	// sent over the network.
	static unsigned NetworkDword(void *ptr);


	PacketFactory();
	Packet* ReadPacket(byte *buffer, int bufferlen, int &packetlen);

private:
	map<byte, Packet*(*)()> _map;
};




enum FieldType {
	UINT32,			// Unsigned 32 bit integer
	INT32,			// Signed 32 bit integer
	FLOAT32,		// 32 bit float
	BYTE,			// 8 bit unsigned
};


class Packet {
public:
	Packet(PacketType type);
	PacketType GetPacketType() const;

	void SetUdpPacket(UDPpacket *udpPacket);

	byte* Serialize(int &pktlen) const;
	int Deserialize(const byte *buf, int buflen);

protected:
	void AddField(FieldType ft, void *ptr);

private:
	PacketType _type;
	vector<pair<FieldType,void*> > _fields;

	/* Used ONLY when the Packet was RECEIVED over UDP
	 * TODO:
	 * Figure out what it does.
	 */
	UDPpacket *_udpPacket;
};


class JoinRequestPacket : public Packet {
public:
	JoinRequestPacket();

	unsigned port;
};


class JoinResponsePacket : public Packet {
public:
	JoinResponsePacket();

	bool response;
	unsigned playerID;
	unsigned team;
};


class PlayerDidJoinPacket : public Packet {
public:
	PlayerDidJoinPacket();

	unsigned playerID;
	unsigned team;
};


class PlayerDidLeavePacket : public Packet {
public:
	PlayerDidLeavePacket();

	unsigned playerID;
};


class ServerShutdownPacket : public Packet {
public:
	ServerShutdownPacket();

};
