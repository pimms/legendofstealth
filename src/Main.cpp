#include <trutle/Trutle.h>
#include "ShadowLayer.h"
#include "LightSource.h"
#include "ShadowCaster.h"
#include "GameController.h"

#ifdef main
#	undef main
#endif

int main(int argc, char *argv[]) 
{
	SDLNet_Init();

	App app;
	app.SetController(new GameController);
	app.Initialize(argc, argv);

	int ret = app.MainLoop();

	SDLNet_Quit();
	return ret;
}
