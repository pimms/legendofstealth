#include "Protocol.h"
#include <stdlib.h>
#include <netinet/in.h>
#include <assert.h>


string PacketTypeStr(PacketType type)
{
	switch (type) {
		case PACKET_UNDEFINED:
			return "PACKET_UNDEFINED";
		case PACKET_JOIN_REQUEST:
			return "PACKET_JOIN_REQUEST";
		case PACKET_JOIN_RESPONSE:
			return "PACKET_JOIN_RESPONSE";
		case PACKET_PLAYER_DID_JOIN:
			return "PACKET_PLAYER_DID_JOIN";
	}
}


/*
================ 
PacketFactory
================ 
*/
int PacketFactory::ReadInt(const byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no); 	

	int i = 0;
	memcpy(&i, &ho, sizeof(int));
	return i;
}

unsigned PacketFactory::ReadUInt(const byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no);

	return ho;
}

float PacketFactory::ReadFloat(const byte *buf)
{
	unsigned no = *((unsigned*) buf);
	unsigned ho = ntohl(no);

	float f = 0.f;

	assert(sizeof f == sizeof ho);

	memcpy(&f, &ho, sizeof(f));
	return f;
}


unsigned PacketFactory::NetworkDword(void *ptr)
{
	unsigned uint = 0;
	memcpy(&uint, ptr, sizeof(uint));
	uint = htonl(uint);
	return uint;
}



template<class T>
Packet* CreatePacketSubclass()
{
	return static_cast<Packet*>(new T());
}


PacketFactory::PacketFactory()
{
	_map[PACKET_JOIN_REQUEST] = &CreatePacketSubclass<JoinRequestPacket>;
	_map[PACKET_JOIN_RESPONSE] = &CreatePacketSubclass<JoinResponsePacket>;
	_map[PACKET_PLAYER_DID_JOIN] = &CreatePacketSubclass<PlayerDidJoinPacket>;
}

Packet* PacketFactory::ReadPacket(byte *buffer, int bufferlen, int &packetlen)
{
	byte type = buffer[0];

	if (_map.count(type) == 0) {
		 char msg[256];
		 sprintf(msg, "Unable to parse packet of type: %i\n", type);
		 Log::Error(msg);
		 return NULL;
	}

	Packet* (*newfunc)() = _map[type];

	Packet *packet = newfunc();
	if (packet) {
		packet->Deserialize(buffer, bufferlen);
	}

	return packet;
}




/*
================ 
Packet
================ 
*/
Packet::Packet(PacketType type)
	:	_type(type),
		_udpPacket(NULL)
{
	AddField(BYTE, &_type);
}

PacketType Packet::GetPacketType() const
{
	return _type;
}

void Packet::SetUdpPacket(UDPpacket *udpPacket)
{
	_udpPacket = udpPacket;
}


byte* Packet::Serialize(int &pktlen) const
{
	vector<byte> buffer;

	for (int i=0; i<_fields.size(); i++) {
		pair<FieldType, void*> field = _fields[i];

		switch (field.first) {
			case UINT32:
			case INT32:
			case FLOAT32:
			{
				unsigned netdw = PacketFactory::NetworkDword(field.second);
				byte *bytes = (byte*) &netdw;

				for (int j=0; j<4; j++) {
					buffer.push_back(bytes[j]);
				}

				break;
			}

			case BYTE:
			{
				byte *bptr = (byte*)field.second;
				buffer.push_back(*bptr);
				break;
			}
		}
	}

	pktlen = buffer.size();

	byte *b = new byte[pktlen];
	for (int i=0; i<buffer.size(); i++) 
		b[i] = buffer[i];

	return b;
}

int Packet::Deserialize(const byte *buf, int buflen)
{
	int read = 0;

	for (int i=0; i<_fields.size(); i++) {
		pair<FieldType, void*> field = _fields[i];
		
		switch (field.first) {
			case UINT32:
			{	
				unsigned *uptr = (unsigned*)field.second;
				(*uptr) = PacketFactory::ReadUInt(buf + read);
				read += sizeof(unsigned);
				break;
			}

			case INT32:
			{	
				int *iptr = (int*)field.second;
				(*iptr) = PacketFactory::ReadInt(buf + read);
				read += sizeof(int);
				break;
			}

			case FLOAT32:
			{	
				float *fptr = (float*)field.second;
				(*fptr) = PacketFactory::ReadFloat(buf + read);
				read += sizeof(float);
				break;
			}

			case BYTE:
			{	
				byte *bptr = (byte*)field.second;
				(*bptr) = buf[read];
				read++;
				break;
			}
		}
	}

	return read;
}


void Packet::AddField(FieldType ft, void *ptr)
{
	_fields.push_back( pair<FieldType,void*>( ft, ptr ) );
}



/*
================
Packet Implementations
================ 
*/
JoinRequestPacket::JoinRequestPacket() 
	:	Packet(PACKET_JOIN_REQUEST)
{
	port = 0;
	AddField(UINT32, &port);
}


JoinResponsePacket::JoinResponsePacket()
	:	Packet(PACKET_JOIN_RESPONSE)
{
	response = false;
	team = 0;
	playerID = 0;

	AddField(BYTE, &response);
	AddField(UINT32, &playerID);
	AddField(UINT32, &team);
}


PlayerDidJoinPacket::PlayerDidJoinPacket()
	:	Packet(PACKET_PLAYER_DID_JOIN)
{
	playerID = 0;
	team = 0;

	AddField(UINT32, &playerID);
	AddField(UINT32, &team);
}
