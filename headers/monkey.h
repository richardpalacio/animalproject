#pragma once

#include "animal.h"

class Monkey : public Animal
{
public:
	Monkey(int id = 0);
	virtual ~Monkey();

	virtual VOID Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};