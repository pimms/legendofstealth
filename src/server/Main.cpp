#include <SDL2/SDL_net.h>
#include <pthread.h>
#include <sstream>
#include <unistd.h>

#ifdef _WIN32
	#include <SDL_net.h>
#else
	#include <SDL2/SDL_net.h>
#endif

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

	Socket *udpSocket = new Socket(UDP, IPaddress{0, UDP_SERVER_PORT}, UDP_SERVER_PORT);

	/* Main Server Loop */
	unsigned npid = 1;
	vector<RemotePlayer*> players;
	unsigned loops = 0;
	int spy = 0, merc = 0;
	while (true) {
		if (listener.HasNewConnection()) {
			Socket *tcp = listener.GetSocket();
			RemotePlayer *rplayer = NULL;
			if (spy > merc) {
				merc++;
				rplayer = new RemotePlayer(npid++, TEAM_MERC, tcp);
			} else {
				spy++;
				rplayer = new RemotePlayer(npid++, TEAM_SPY, tcp);
			}

			for (int i=0; i<players.size(); i++) {
				// Notify the other players of the new dude
				PacketPlayerDidJoin pkt;
				pkt.type = PACKET_PLAYER_DID_JOIN;
				pkt.team = (unsigned)rplayer->GetTeam();
				pkt.playerID = rplayer->GetPlayerID();
				players[i]->SendPacket(TCP, &pkt);

				// Notify the new player of all the existing guys
				pkt.team = players[i]->GetTeam();
				pkt.playerID = players[i]->GetPlayerID();
				rplayer->SendPacket(TCP, &pkt);
			}

			players.push_back(rplayer);

			std::stringstream ss;
			ss << "Player joined: " << rplayer->GetPlayerID();
			Log::Debug(ss.str());
		}

		for (int i=0; i<players.size(); i++) {
			players[i]->Update();
			if (!players[i]->IsConnected()) {
				if (players[i]->GetTeam() == TEAM_SPY) {
					spy--;
				} else {
					merc--;
				}
				std::stringstream ss;
				ss << "Player left: " << players[i]->GetPlayerID();
				Log::Debug(ss.str());

				delete players[i];
				players.erase(players.begin() + i--);
			}
		}

		while (udpSocket->HasActivity()) {
			Packet *pkt = udpSocket->GetPacket();
			if (pkt && pkt->type != PACKET_PLAYER_UPDATE) {
				Log::Debug("Received UDP packet: " + PacketTypeStr(pkt->type));
			}

			if (pkt && pkt->type == PACKET_PLAYER_UPDATE) {
				PacketPlayerUpdate *ppu = (PacketPlayerUpdate*)pkt;
				for (int i=0; i<players.size(); i++) {
					if (players[i]->GetPlayerID() == ppu->playerID) {
						players[i]->HandleUpdatePacket(ppu);
					}
				}
			}
		}

		for (int i=0; i<players.size(); i++) {
			for (int j=0; j<players.size(); j++) {
				if (j == i) continue;
				
				PacketPlayerUpdate pkt = players[i]->CreateUpdatePacket();
				players[j]->SendPacket(UDP, &pkt);			
			}
		}

		usleep(50000);

	}

	for (int i=0; i<players.size(); i++) {
		delete players[i];
	}

	delete udpSocket;

	SDLNet_Quit();
	return 0;
}
