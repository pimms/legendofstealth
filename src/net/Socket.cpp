#include "Socket.h"
#include <cstdlib>
#include <stdlib.h>


/*
================
Socket Public 
================
*/
Socket::Socket(Protocol protocol, string hostname, int port, unsigned udpListenPort) 
	:	_set(NULL),
		_udp(0),
		_tcp(0),
		_protocol(protocol),
		_hostname(hostname),
		_udpListenPort(udpListenPort)
{
	if (SDLNet_ResolveHost(&_ip, _hostname.c_str(), port) == -1) {
		string msg = (string)"Failed to resolve host " + _hostname;
		Log::Error(msg);
		throw std::runtime_error(msg);
	}

	CreateSocketSet();

	if (_protocol == TCP) {
		ConnectTCP();
		if (SDLNet_TCP_AddSocket(_set, _tcp) == -1) {
			string msg = "Failed to add socket to set";
			Log::Error(msg);
			throw std::runtime_error(msg);
		}
	} else if (_protocol == UDP) {
		CreateUDP();
		if (SDLNet_UDP_AddSocket(_set, _udp) == -1) {
			string msg = "Failed to add socket to set";
			Log::Error(msg);
			throw std::runtime_error(msg);
		}
	}
	

}	

Socket::~Socket()
{
	if (_set) {
		SDLNet_FreeSocketSet(_set);
	}
}


bool Socket::HasActivity()
{
	if (_pqueue.size())
		return true;

	int numready = SDLNet_CheckSockets(_set, 0);
	if (numready) {
		if (_protocol == UDP) {
			return SDLNet_SocketReady(_udp);
		} else if (_protocol == TCP) {
			return SDLNet_SocketReady(_tcp);
		}
	}

	return false;
}

Packet* Socket::GetPacket()
{
	if (_pqueue.size()) {
		Packet *p = _pqueue[0];
		_pqueue.erase(_pqueue.begin());
		return p;
	}

	if (!HasActivity())
		return NULL;

	int len = 0;
	byte *buf = NULL;
	UDPpacket *udpPacket = NULL;

	buf = GetData(len, udpPacket);
	if (!buf) 
		return NULL;

	while (len > 0) {
		int plen = 0;
		Packet *packet = Packet::ReadPacket(buf, len, plen);
		
		if (packet) {
			packet->udpPacket = udpPacket;
			len -= plen;
			_pqueue.push_back(packet);
		} else {
			// Unable to parse packet - none of the retrieved data can be trusted
			if (udpPacket) 
				SDLNet_FreePacket(udpPacket);
			break;
		}
	}

	if (_pqueue.size())
		return GetPacket();

	return NULL;
}


bool Socket::SendPacket(Packet *packet)
{
	int len = 0;
	int sent = 0;
	byte *buf = NULL;

	buf = packet->GetSendablePacket(len);
	
	if (_protocol == TCP) {
		sent = SDLNet_TCP_Send(_tcp, buf, len);
	} else if (_protocol == UDP) {
		UDPpacket pkt;
		pkt.data = buf;
		pkt.len = len;
		pkt.address = _ip;

		sent = SDLNet_UDP_Send(_udp, -1, &pkt);
	}

	if (sent == 0) {
		string msg = "Failed to send ";
		msg += (string)( (_protocol==TCP)? "TCP" : "UDP" );
		msg += (string)" packet. Error: " + SDLNet_GetError();
		Log::Error(msg);
		return false;
	}

	return true;
}



/*
================
Socket Private
================
*/
void Socket::CreateSocketSet()
{
	if (!_set) {
		_set = SDLNet_AllocSocketSet(1);
		if (!_set) {
			string msg = "Failed to allocate socket set";
			Log::Error(msg);
			throw std::runtime_error(msg);
		}
	}
}


byte* Socket::GetData(int &len, UDPpacket*& udpPacket)
{
	len = 0;
	byte *buf = NULL;
	int num = 0;

	if (_protocol == UDP) {
		UDPpacket *packet = SDLNet_AllocPacket(1024);

		num = SDLNet_UDP_Recv(_udp, packet);
		if (num == 1) {
			if (packet->len == packet->maxlen) {
				Log::Error("Received large packet (>1K). Errors WILL occur!");
			}

			buf = new byte[packet->len];
			memcpy(buf, packet->data, packet->len);
			len = packet->len;
		}
		
		udpPacket = packet;
	} else if (_protocol == TCP) {
		int maxlen = 128;
		int totread = 0;
		buf = new byte[maxlen];
		num = SDLNet_TCP_Recv(_tcp, buf, maxlen);
		totread = num;
		
		while (num == maxlen) {
			totread += num;
			maxlen *= 2;
			buf = (byte*)realloc(buf, maxlen);
			num = SDLNet_TCP_Recv(_tcp, buf+totread, maxlen);
		}

		len = totread;
	}

	return buf;
}


void Socket::ConnectTCP()
{
	_tcp = SDLNet_TCP_Open(&_ip);
	if (!_tcp) {
		string msg = (string)"Failed to connect to host " + _hostname;
		Log::Error(msg);
		throw runtime_error(msg);
	}

	Log::Debug("Opened TCP connection to " + _hostname);
}

void Socket::CreateUDP()
{
	_udp = SDLNet_UDP_Open(_udpListenPort);
	if (!_udp) {
		char buf[64];
		sprintf(buf, "Failed to bind UDP port %i", _udpListenPort);
		Log::Error(buf);
	}

	// In case _udpListenPort is 0, retrieve the port used by the socket
	IPaddress *sockip = SDLNet_UDP_GetPeerAddress(_udp, -1);
	_udpListenPort = sockip->port;
	
	char buf[64];
	sprintf(buf, "Bound UDP on port %i", _udpListenPort);
	Log::Debug(buf);
}
