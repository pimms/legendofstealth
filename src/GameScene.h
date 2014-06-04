#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif




class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	void LoadContent();
	void Update(const DeltaTime &dt);

private:
};
