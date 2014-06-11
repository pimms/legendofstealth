#include <iostream>
#include <trutle/Trutle.h>
#include "GameScene.h"

int main(int argc, char **argv) 
{
	App app;
	app.Initialize(argc, argv);
	
	Controller *controller = new Controller();
	app.SetController(controller);
	
	Scene *scene = new GameScene();
	controller->SetScene(scene);
	
    return app.MainLoop();
}
