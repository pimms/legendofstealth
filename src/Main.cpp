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
	App app;
	app.SetController(new GameController);
	app.Initialize(argc, argv);

	return app.MainLoop();
}
