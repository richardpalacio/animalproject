#pragma once

#include "entity.h"
#include "cardiovascularsystem.h"

class Corporeal : public Entity
{
public:
	Corporeal(int id = 0);
	virtual ~Corporeal();
	
	virtual void Eat() const = 0;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
protected:
	bool sleeping;

	CardiovascularSystem m_pCardioVascularSystem;
};