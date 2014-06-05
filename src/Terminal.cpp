#include "Terminal.h"
#include "Hack.h"

Terminal::Terminal(b2World *world, string texture, Vec2 pos) : Entity(world) {
	LoadTexture(texture);
	Position() = pos;	
	AddComponent<Hackable>(this);
}

Terminal::~Terminal() {

}