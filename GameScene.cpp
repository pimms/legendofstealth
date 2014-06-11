#include "GameScene.h"
#include "GameLayer.h"

GameScene::GameScene()
	:	_gameLayer(NULL)
{
	
}


void GameScene::LoadContent()
{
	_gameLayer = new GameLayer();
	AddLayer(_gameLayer);
}

void GameScene::Update(const DeltaTime &dt)
{
	Scene::Update(dt);
}

void GameScene::Render(Renderer* renderer)
{
	Scene::Render(renderer);
}
