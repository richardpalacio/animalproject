#pragma once

#include "animal.h"

class Panda : public Animal
{
public:
	Panda(int id = 0);
	virtual ~Panda();

	virtual VOID Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};