#include "corporeal.h"

Corporeal::Corporeal(int id)
	:Entity(id)
{
}

Corporeal::~Corporeal()
{
}

VOID Corporeal::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	this->Entity::Update(deltaTime, currentCamera);

	m_pCardioVascularSystem.Update(deltaTime);
}