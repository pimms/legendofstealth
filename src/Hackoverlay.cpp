#include "Hackoverlay.h"

Hackoverlay::Hackoverlay() 
	:	_go(NULL)
{
	_go = new GameObject();
	AddChild(_go);
}

Hackoverlay::~Hackoverlay() {

}


void Hackoverlay::SetTexture(string texture)
{
	_go->LoadTexture(texture);
}
