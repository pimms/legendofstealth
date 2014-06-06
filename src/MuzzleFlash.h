#pragma once

#include <trutle/Trutle.h>


class MuzzleFlash : public GameObject {
public:
	MuzzleFlash();
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);
	
	void Show();

private:
	float _timer;
};



class BloodSplat : public GameObject {
public:
	BloodSplat();
	void Update(const DeltaTime &dt);
	void Render(Renderer *renderer);
	
	void Show();

private:
	float _timer;
};
