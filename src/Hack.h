#pragma once

#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#define HACKTIME 100;
#define RESET_TIME 10;

class Hack : public Component {
	public:
		Hack();

		void Update(const DeltaTime &dt);

	private:
		bool _hacking = false, _hackdone = false;
		float _hacktime = HACKTIME;
		float _resettime = RESET_TIME;

		void ResetCount();

};