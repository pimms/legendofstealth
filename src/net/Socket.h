#pragma once

#include <trutle/Trutle.h>
#include <SDL2/SDL_net.h>
#include "Packet.h"


/* Socket
 *
 * Hides the SDLNet interface and the underlying transport layer protocol. 
 * Only data of type Packet can be sent and read. If multiple packets were
 * sent to the Socket since last checking it, they are queued and returned
 * before the queue is cleared again. 
 *
 * HasActivity() will return TRUE if there are Packets on the packet-queue 
 * or there is remote data waiting.
 */
class Socket {
public:
	/* Connect to the remote host
	 */
	Socket(Protocol protocol, string hostname, int port, unsigned udpListenPort=0);
	Socket(Protocol protocol, IPaddress ipaddress, unsigned udpListenPort=0);

	/* Connection has already been set up
	 */
	Socket(TCPsocket tcpsocket);

	~Socket();

	bool HasActivity();
	Packet* GetPacket();

	bool SendPacket(Packet *packet);

	// Returns an empty string on UDP Sockets
	string GetRemoteHostname() const;

	unsigned GetListenPortUDP() const;
	Protocol GetProtocol() const;

	// May not return a valid value under UDP
	IPaddress GetIPaddress();

	static string GetOctalIP(Uint32 ip);

	// Only valid for use with TCP sockets
	bool IsConnectionOpen();

private:
	SDLNet_SocketSet _set;	
	Protocol _protocol;
	vector<Packet*> _pqueue;
	unsigned _udpListenPort;

	string _hostname;
	TCPsocket _tcp;
	UDPsocket _udp;
	IPaddress _ip;
	
	void CreateSocketSet();
	byte* GetData(int &len, UDPpacket *&udpPacket);

	void ConnectTCP();
	void CreateUDP();

	void Init();

	bool _tcpDisconnect;
};

class UDPSocket : public Socket {
public:

private:

};

class SocketTCP: public Socket {
public:
	SocketTCP(string hostname, unsigned port);
	SocketTCP(IPaddress *ipaddr);

private:
	string _hostname;
};
