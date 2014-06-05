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
#include "Server.h"


#ifdef main
#	undef main
#endif

int main(int argc, char *argv[]) 
{
	SDLNet_Init();

	Server server;
	server.ServerLoop();

	SDLNet_Quit();
	return 0;
}
