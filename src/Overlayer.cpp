#include "Overlayer.h"

Overlayer::Overlayer() {
	//DisplayTexture("res/red.png");
}

Overlayer::~Overlayer() {}

void Overlayer::DisplayTexture(string texture) {
	LoadTexture(texture);
	DisplayTexture(texture);
}