#include "monkey.h"

Monkey::Monkey(int id)
	:Animal(id)
{
}

Monkey::~Monkey()
{
	cout << "monkey destructor called" << endl;
}

VOID Monkey::Eat() const
{
	cout << "monkey eating" << endl;
}

VOID Monkey::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Animal::Update(deltaTime, currentCamera);

	m_pCardioVascularSystem.Update(deltaTime);
}