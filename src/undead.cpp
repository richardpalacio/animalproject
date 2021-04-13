#include "undead.h"

Undead::Undead(int id)
	:Corporeal(id)
{
}

Undead::~Undead()
{
}

VOID Undead::Eat() const
{
	cout << "undead eating" << endl;
}

VOID Undead::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Corporeal::Update(deltaTime, currentCamera);
}
