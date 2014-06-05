#include "Terminal.h"

Terminal::Terminal(b2World *world, string texture) : Entity(world) {
	LoadTexture(texture);
	Position() = Vec2(100.0f, 100.0f);	
	AddComponent<PlayerUpdateComponent>(this);
}

Terminal::~Terminal() {

}