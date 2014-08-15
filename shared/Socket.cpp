#include "Socket.h"
#include <cstdlib>
#include <stdlib.h>
#include <sstream>


/*
================
Socket Public
================
*/
Socket::Socket(TLProtocol protocol, string hostname, int port, unsigned udpListenPort)
	:	_set(NULL),
		_udp(0),
		_tcp(0),
		_protocol(protocol),
		_hostname(hostname),
		_udpListenPort(udpListenPort),
		_tcpDisconnect(false)
{
	if (SDLNet_ResolveHost(&_ip, _hostname.c_str(), port) == -1) {
		string msg = (string)"Failed to resolve host " + _hostname;
		Log::Error(msg);
		throw std::runtime_error(msg);
	}

	Init();
}

Socket::Socket(TLProtocol protocol, IPaddress ipaddr, unsigned udpListenPort)
	:	_set(0),
		_udp(0),
		_tcp(0),
		_protocol(protocol),
		_hostname(GetOctalIP(ipaddr.host)),
		_udpListenPort(udpListenPort),
		_tcpDisconnect(false),
		_ip(ipaddr)
{
	Init();
}



Socket::Socket(TCPsocket tcpsocket)
	:	_set(NULL),
		_udp(0),
		_tcp(tcpsocket),
		_protocol(TCP),
		_hostname(""),
		_udpListenPort(0),
		_tcpDisconnect(false)
{
	IPaddress *ip = SDLNet_TCP_GetPeerAddress(_tcp);
	memcpy(&_ip, ip, sizeof(IPaddress));

	_hostname = GetOctalIP(_ip.host);
	Log::Debug("Socket created with existing connection from " + _hostname);

	CreateSocketSet();
	if (SDLNet_TCP_AddSocket(_set, _tcp) == -1) {
		string msg = "Failed to add TCP socket to set";
		Log::Error(msg);
		throw std::runtime_error(msg);
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
	if (!buf || !len) {
		_tcpDisconnect = (_protocol == TCP);
		Log::Verbose("TCP socket disconnected");
		return NULL;
	}

	PacketFactory pktFactory;
	int numRecv = 0;
	int read = 0;

	while (len > 0) {
		int plen = 0;
		Packet *packet = pktFactory.ReadPacket(buf+read, len, plen);

		if (packet) {
			packet->SetUdpPacket(udpPacket);
			len -= plen;
			read += plen;
			_pqueue.push_back(packet);

			numRecv++;
		} else {
			// Unable to parse packet - none of the retrieved data can be trusted
			if (udpPacket)
				SDLNet_FreePacket(udpPacket);
			Log::Verbose("Received non-trustworthy data");
			break;
		}
	}

	for (int i=0; i<numRecv; i++) {
		PacketType t = _pqueue[_pqueue.size()-1-i]->GetPacketType();
		Log::Verbose("Received packet: " + PacketTypeStr(t));
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

	buf = packet->Serialize(len);
	if (!buf)
		return false;

	if (_protocol == TCP) {
		sent = SDLNet_TCP_Send(_tcp, buf, len);
	} else if (_protocol == UDP) {
		UDPpacket pkt;
		pkt.data = buf;
		pkt.len = len;
		pkt.address = _ip;
		pkt.channel = 0;

		sent = SDLNet_UDP_Send(_udp, pkt.channel, &pkt);
	}

	delete[] buf;

	if (sent == 0) {
		string msg = "Failed to send ";
		msg += (string)( (_protocol==TCP)? "TCP" : "UDP" );
		msg += (string)" packet. Error: " + SDLNet_GetError();
		Log::Error(msg);
		return false;
	} else {
		PacketType t = packet->GetPacketType();
		Log::Verbose("Received packet: " + PacketTypeStr(t));
	}

	return true;
}



string Socket::GetOctalIP(Uint32 ip)
{
	std::stringstream ss;
	ss << ((ip >> 24) & 0xFF) << ".";
	ss << ((ip >> 16) & 0xFF) << ".";
	ss << ((ip >> 8 ) & 0xFF) << ".";
	ss << ((ip      ) & 0xFF);
	return ss.str();
}


string Socket::GetRemoteHostname() const
{
	if (_protocol == UDP) {
		Log::Debug("Hostname not available for UDP connections");
		return "";
	}

	return _hostname;
}

unsigned Socket::GetListenPortUDP() const
{
	return _udpListenPort;
}

TLProtocol Socket::GetTransportLayerProtocol() const
{
	return _protocol;
}

IPaddress Socket::GetIPaddress()
{
	return _ip;
}


bool Socket::IsConnectionOpen()
{
	if (_tcpDisconnect)
		return false;

	return (bool)_tcp;
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
		std::stringstream ss;
		ss << "Failed to bind UDP port " << _udpListenPort;
		Log::Error(ss.str());
	}

	// In case _udpListenPort is 0, retrieve the port used by the socket
	IPaddress *sockip = SDLNet_UDP_GetPeerAddress(_udp, -1);
	_udpListenPort = sockip->port;

	std::stringstream ss;
	ss << "Bound UDP socket on port " << _udpListenPort;
	Log::Debug(ss.str());
}


void Socket::Init()
{
	CreateSocketSet();

	if (_protocol == TCP) {
		ConnectTCP();
		if (SDLNet_TCP_AddSocket(_set, _tcp) == -1) {
			string msg = "Failed to add TCP socket to set";
			Log::Error(msg);
			throw std::runtime_error(msg);
		}
	} else if (_protocol == UDP) {
		CreateUDP();
		if (SDLNet_UDP_AddSocket(_set, _udp) == -1) {
			string msg = "Failed to add UDP socket to set";
			Log::Error(msg);
			throw std::runtime_error(msg);
		}

		int chan = SDLNet_UDP_Bind(_udp, 0, &_ip);
		if (chan == -1) {
			string msg = (string)"Failed to bind address " + _hostname;
			Log::Error(msg);
			throw std::runtime_error(msg);
		} else {
			Log::Debug("Successfully bound address " + _hostname + " to channel 0");
		}
	}
}
