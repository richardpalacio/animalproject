#pragma once

#include "corporeal.h"

class Undead : public Corporeal
{
public:
	Undead(int id);
	virtual ~Undead();

	virtual void Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};
