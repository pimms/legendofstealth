#pragma once

#include <trutle/Trutle.h>


class GameController : public Controller {
public:
	GameController();
	~GameController();

	void LoadContent();
	void Update(DeltaTime &dt);

private:

};
