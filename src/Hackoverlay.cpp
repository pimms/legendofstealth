#include "Hackoverlay.h"

Hackoverlay::Hackoverlay(b2World *world, string texture, Vec2 pos) : Entity(world) {
	LoadTexture(texture);
	Position() = pos;
}

Hackoverlay::~Hackoverlay() {

}