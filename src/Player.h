#pragma once

// Windows and Unix specific includes
#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

class Player : public GameObject{
public:
	Player();
private:

	
};

