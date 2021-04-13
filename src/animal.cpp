#include "animal.h"

Animal::Animal(int id)
	:Corporeal(id)
{

}

Animal::~Animal()
{
	cout << "animal destructor called" << endl;
}

VOID Animal::Eat() const
{
	cout << "animal eating" << endl;
}

VOID Animal::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Corporeal::Update(deltaTime, currentCamera);
}

Animal Animal::operator +(Animal const &cAnimalInstance)
{
	return cAnimalInstance;
}

ostream& operator <<(ostream &ostreamInstance, const Animal &animalInstance)
{
	animalInstance.Eat();

	return ostreamInstance;
}