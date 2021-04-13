#include "panda.h"

Panda::Panda(int id)
	:Animal(id)
{
}

Panda::~Panda()
{
	cout << "panda destructor called" << endl;
}

VOID Panda::Eat() const
{
	cout << "panda eating" << endl;
}

VOID Panda::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Animal::Update(deltaTime, currentCamera);

	m_pCardioVascularSystem.Update(deltaTime);
}