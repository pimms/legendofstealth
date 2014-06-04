#pragma once

// Windows and Unix specific includes
#ifdef _WIN32
	#include "Trutle.h"
#else
	#include <trutle/Trutle.h>
#endif

#include "Entity.h"
#include "PlayerUpdateComponent.h"

class Player : public Entity {
public:
	Player(b2World *world, bool isLocal);

	void SetPlayerID(unsigned id);
	unsigned GetPlayerID() const;

	void Update(const DeltaTime &dt);

private:
	unsigned _playerID;
	bool _local;
	PlayerUpdateComponent *_updateComponent;
};

