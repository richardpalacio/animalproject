#include "zombie.h"

Zombie::Zombie(int id)
	:Undead(id)
{

}

Zombie::~Zombie()
{
	cout << "zombie destructor called" << endl;
}

VOID Zombie::Eat() const
{
	cout << "zombie eating" << endl;
}

VOID Zombie::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Undead::Update(deltaTime, currentCamera);
}