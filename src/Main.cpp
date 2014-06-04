#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include "ShadowLayer.h"
#include "LightSource.h"
#include "ShadowCaster.h"
#include "GameController.h"

#ifdef main
#	undef main
#endif

string addr = "localhost";
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[]) 
{
	SDLNet_Init();

	ParseArgs(argc, argv);

	App app;
	app.SetController(new GameController(addr));
	app.Initialize(argc, argv);

	int ret = app.MainLoop();

	SDLNet_Quit();
	return ret;
}

void ParseArgs(int argc, char **argv) {
	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-addr"))
		{
			addr = argv[++i];
		}
	}
}