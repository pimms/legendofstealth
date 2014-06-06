#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"

class Hackoverlay : public Layer {
public:
	Hackoverlay();
	~Hackoverlay();

	void SetTexture(string texture);

private:
	GameObject *_go;
};

