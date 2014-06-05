

#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Packet.h"
#include <stdlib.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
#endif

#include <assert.h>


string PacketTypeStr(PacketType type)
{
	switch (type) {
		case PACKET_JOIN_REQUEST:
			return "PACKET_JOIN_REQUEST";
			break;
		case PACKET_JOIN_RESPONSE:
			return "PACKET_JOIN_RESPONSE";
			break;
		case PACKET_PLAYER_DID_JOIN:
			return "PACKET_PLAYER_DID_JOIN";
			break;
		case PACKET_PLAYER_DID_LEAVE:
			return "PACKET_PLAYER_DID_LEAVE";
			break;
		case PACKET_PLAYER_UPDATE:
			return "PACKET_PLAYER_UPDATE";
			break;
		case PACKET_PLAYER_FIRE:
			return "PACKET_PLAYER_FIRE";
			break;
		case PACKET_PLAYER_HIT:
			return "PACKET_PLAYER_HIT";
			break;
		case PACKET_PLAYER_HACK:
			return "PACKET_PLAYER_HACK";
			break;
	}
}


Packet* Packet::ReadPacket(byte *buffer, int bufferlen, int &packetlen)
{
	packetlen = 0;
	Packet *packet = NULL;

	switch (buffer[0]) {
		case PACKET_JOIN_REQUEST:
			packet = new PacketJoinRequest;
			break;
		case PACKET_JOIN_RESPONSE:
			packet = new PacketJoinResponse;
			break;
		case PACKET_PLAYER_DID_JOIN:
			packet = new PacketPlayerDidJoin;
			break;
		case PACKET_PLAYER_DID_LEAVE:
			packet = new PacketPlayerDidLeave;
			break;
		case PACKET_PLAYER_UPDATE:
			packet = new PacketPlayerUpdate;
			break;
		case PACKET_PLAYER_FIRE:
			packet = new PacketPlayerFire;
			break;
		case PACKET_PLAYER_HIT:
			packet = new PacketPlayerHit;
			break;
		case PACKET_PLAYER_HACK:
			packet = new PacketPlayerHack;
			break;

		default:
			char b[4];
			sprintf(b, "%i", buffer[0]);
			string msg = (string)"Could not identify packet with ID: " + b;
			Log::Warning(msg);
			return NULL;
	}

	packet->type = (PacketType)buffer[0];
	packetlen = packet->FillPacketFromBuffer(buffer, bufferlen);
	return packet;
}

int Packet::ReadInt(byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no); 	

	int i = 0;
	memcpy(&i, &ho, sizeof(int));
	return i;
}

unsigned Packet::ReadUInt(byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no);

	return ho;
}

float Packet::ReadFloat(byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no);

	float f = 0.f;

	assert(sizeof f == sizeof ho);

	memcpy(&f, &ho, sizeof(f));
	return f;
}

Packet::Packet()
	:	udpPacket(NULL)
{

}

Packet::~Packet()
{
	if (udpPacket) {
		SDLNet_FreePacket(udpPacket);
	}
}


/* The filling and writing of a struct from buffer is done via 
 * macros because this is a game jam. yolo, etc. See existing usages 
 * for usage info.
 */

#define READ_FLOAT(FIELD)				\
	FIELD = ReadFloat(buffer + read); 	\
	read += sizeof(float);

#define READ_INT(FIELD) 				\
	FIELD = ReadInt(buffer + read);		\
	read += sizeof(int);

#define READ_UINT(FIELD) 				\
	FIELD = ReadUInt(buffer + read);	\
	read += sizeof(unsigned);

#define READ_BYTE(FIELD) 				\
	FIELD = buffer[read];				\
	read++;

#define FILL_METHOD_BEGIN(STRUCT)		\
	int STRUCT::FillPacketFromBuffer(byte *buffer, int len) { \
		type = (PacketType)buffer[0];	\
		int read = 1;					

#define FILL_METHOD_END					\
		return read;					\
	}


#define WRITE_LONG(FIELD) 				\
	{  	unsigned u = 0;					\
		memcpy(&u, &FIELD, 4);			\
		u = htonl(u);					\
		memcpy(buf + packetlen, &u, 4);	\
		packetlen += 4;		}			

#define WRITE_BYTE(FIELD)				\
		buf[packetlen] = FIELD;			\
		packetlen++;

#define WRITE_METHOD_BEGIN(STRUCT) 		\
	byte* STRUCT::GetSendablePacket(int &packetlen) { \
		packetlen = 1;					\
		byte buf[128]; 					\
		buf[0] = (byte)type; 			

#define WRITE_METHOD_END				\
		byte *pkg = new byte[packetlen];\
		memcpy(pkg, buf, packetlen);	\
		return pkg;						\
	}

// PacketJoinRequest
FILL_METHOD_BEGIN(PacketJoinRequest)
READ_UINT(port)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketJoinRequest)
WRITE_LONG(port)
WRITE_METHOD_END

// PacketJoinResponse
FILL_METHOD_BEGIN(PacketJoinResponse)
READ_BYTE(response)
READ_UINT(playerID)
READ_UINT(team)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketJoinResponse)
WRITE_BYTE(response)
WRITE_LONG(playerID)
WRITE_LONG(team)
WRITE_METHOD_END

// PacketPlayerDidJoin 
FILL_METHOD_BEGIN(PacketPlayerDidJoin)
READ_UINT(playerID)
READ_UINT(team)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerDidJoin)
WRITE_LONG(playerID)
WRITE_LONG(team)
WRITE_METHOD_END

// PacketPlayerDidLeave 
FILL_METHOD_BEGIN(PacketPlayerDidLeave)
READ_UINT(playerID)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerDidLeave)
WRITE_LONG(playerID)
WRITE_METHOD_END

// PacketPlayerUpdate
FILL_METHOD_BEGIN(PacketPlayerUpdate)
READ_UINT(counter)
READ_UINT(playerID)
READ_FLOAT(posX)
READ_FLOAT(posY)
READ_FLOAT(rotation)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerUpdate)
WRITE_LONG(counter)
WRITE_LONG(playerID)
WRITE_LONG(posX)
WRITE_LONG(posY)
WRITE_LONG(rotation)
WRITE_METHOD_END

// PacketPlayerFire
FILL_METHOD_BEGIN(PacketPlayerFire)
READ_UINT(playerID)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerFire)
WRITE_LONG(playerID)
WRITE_METHOD_END

// PacketPlayerHit
FILL_METHOD_BEGIN(PacketPlayerHit)
READ_UINT(playerID)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerHit)
WRITE_LONG(playerID)
WRITE_METHOD_END

// PacketPlayerHack
FILL_METHOD_BEGIN(PacketPlayerHack)
READ_UINT(playerID)
READ_UINT(terminalID)
READ_BYTE(isHacking)
FILL_METHOD_END

WRITE_METHOD_BEGIN(PacketPlayerHack)
WRITE_LONG(playerID)
WRITE_LONG(terminalID)
WRITE_BYTE(isHacking)
WRITE_METHOD_END
