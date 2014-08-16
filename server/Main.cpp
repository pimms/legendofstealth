#include <SDL2/SDL_net.h>
#include <pthread.h>
#include <sstream>
#include <unistd.h>

#include <SDL2/SDL_net.h>

#include "Server.h"


#ifdef main
#	undef main
#endif

#ifndef IS_TESTING
int main(int argc, char *argv[]) 
{
	SDLNet_Init();

	server::Server server;
	server.ServerLoop();

	SDLNet_Quit();
	return 0;
}
#endif
