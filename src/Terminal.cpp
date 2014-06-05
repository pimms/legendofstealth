#include "Terminal.h"
#include "Hack.h"

Terminal::Terminal(b2World *world, string texture) : Entity(world) {
	LoadTexture(texture);
	Position() = Vec2(200.0f, 100.0f);	
	AddComponent<Hackable>(this);
}

Terminal::~Terminal() {

}