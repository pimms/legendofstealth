#include "MuzzleFlash.h"


MuzzleFlash::MuzzleFlash()
	:	_timer(0.f)
{
	LoadTexture("res/muzzleflash.png");
	Rotation() = 90.f;
	Position() = Vec2(25.f, 0.f);
}

void MuzzleFlash::Update(const DeltaTime &dt)
{
	_timer -= dt.dt;
}

void MuzzleFlash::Render(Renderer *renderer)
{
	if (_timer >= 0.f)
		GameObject::Render(renderer);
}

void MuzzleFlash::Show()
{
	_timer = 0.25f;
}




BloodSplat::BloodSplat()
	:	_timer(0.f)
{
	LoadTexture("res/blood.png");
}

void BloodSplat::Update(const DeltaTime &dt)
{
	_timer -= dt.dt;
}

void BloodSplat::Render(Renderer *renderer)
{
	if (_timer >= 0.f)
		GameObject::Render(renderer);
}

void BloodSplat::Show()
{
	_timer = 0.25f;
}
