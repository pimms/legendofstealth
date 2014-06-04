#include <SDL2/SDL_net.h>
#include <pthread.h>
#include <sstream>

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

	/* Main Server Loop */
	unsigned npid = 1;
	vector<RemotePlayer*> players;
	while (true) {
		if (listener.HasNewConnection()) {
			Socket *tcp = listener.GetSocket();
			RemotePlayer *rplayer = new RemotePlayer(npid++, tcp);
			players.push_back(rplayer);

			std::stringstream ss;
			ss << "Player joined: " << rplayer->GetPlayerID();
			Log::Debug(ss.str());
		}

		for (int i=0; i<players.size(); i++) {
			players[i]->Update();
			if (!players[i]->IsConnected()) {
				std::stringstream ss;
				ss << "Player left: " << players[i]->GetPlayerID();
				Log::Debug(ss.str());

				delete players[i];
				players.erase(players.begin() + i--);
			}
		}
	}

	for (int i=0; i<players.size(); i++) {
		delete players[i];
	}

	SDLNet_Quit();
	return 0;
}
