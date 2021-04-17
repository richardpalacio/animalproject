#include "corporeal.h"

Corporeal::Corporeal(int id)
	:Entity(id), sleeping(false)
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