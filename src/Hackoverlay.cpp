#include "Hackoverlay.h"

Hackoverlay::Hackoverlay() 
	:	_go(NULL)
{
	_go = new GameObject();
	AddChild(_go);

	_go->Pivot() = Vec2(0.f, 0.f);
}

Hackoverlay::~Hackoverlay() {

}


void Hackoverlay::SetTexture(string texture)
{
	_go->LoadTexture(texture);
	AddChild(_go);
}

void Hackoverlay::HideTexture()
{
	RemoveChild(_go);
}
