#include "incorporeal.h"

Incorporeal::Incorporeal(int id)
	:Entity(id)
{
}

Incorporeal::~Incorporeal()
{
}

VOID Incorporeal::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Entity::Update(deltaTime, currentCamera);
}