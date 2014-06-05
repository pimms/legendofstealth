#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

class Overlayer : public Layer {
	public:
		Overlayer();
		~Overlayer();

		void DisplayTexture(string texture);

	private:
};