#pragma once

#include <trutle/Trutle.h>


class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void LoadContent();
	void Update(const DeltaTime &dt);

private:
};
