#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif




class FireComponent : public Component {
public:
	FireComponent();
	void update();

private:
	bool lineOfSight(float playerX, 
					 float playerY, 
					 float enemyX, 
					 float enemyY
					 );
};