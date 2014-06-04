#include <SDL2/SDL_net.h>
#include <pthread.h>

#include "ConnectionListener.h"
#include "../net/Socket.h"
#include "RemotePlayer.h"

#ifdef main
#	undef main
#endif

int main(int argc, char *argv[]) 
{
	SDLNet_Init();

	ConnectionListener listener;

	while (!listener.HasNewConnection()) 	
		/* null */ ;
	Socket *tcpsock = listener.GetSocket();	
	Socket *udpsock = NULL;

	while (true) {
		if (tcpsock && tcpsock->HasActivity()) {
			Packet *p = tcpsock->GetPacket();
			if (p) {
				printf("Received TCP packet of type %i\n", p->type);

				if (p->type == PACKET_JOIN_REQUEST && !udpsock) {
					PacketJoinRequest *pjr = (PacketJoinRequest*)p;
					udpsock = new Socket(UDP, tcpsock->GetRemoteHostname(), pjr->port, 0);
					printf("Created UDP socket %s:%i\n", tcpsock->GetRemoteHostname().c_str(), pjr->port);
				}
				
				delete p;
			}

			if (!tcpsock->IsConnectionOpen()) {
				delete tcpsock;
				tcpsock = NULL;
			}
		}

		if (udpsock && udpsock->HasActivity()) {
			Packet *p = udpsock->GetPacket();
			if (p) {
				printf("Received UDP packet of type %i\n", p->type);
				delete p;
			}
		}
	}


	/* Main Server Loop */
	unsigned npid = 1;
	vector<RemotePlayer*> players;
	while (true) {
		if (listener.HasNewConnection()) {
			Socket *tcp = listener.GetSocket();
			RemotePlayer *rplayer = new RemotePlayer(npid++, tcp);
			players.push_back(rplayer);
		}

		for (int i=0; i<players.size(); i++) {
			
		}
	}

	delete tcpsock;
	SDLNet_Quit();
	return 0;
}
