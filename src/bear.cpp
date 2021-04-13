#include "bear.h"

Bear::Bear(int id)
	:Animal(id)
{

}

Bear::~Bear()
{
	cout << "bear destructor called" << endl;
}

VOID Bear::Eat() const
{
	cout << "bear eating" << endl;
}

VOID Bear::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Animal::Update(deltaTime, currentCamera);
}