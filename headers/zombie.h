#pragma once

#include "undead.h"

/*
idbeastiary 2
*/
class Zombie : public Undead
{
public:
	Zombie(int id = 2);
	virtual ~Zombie();

	virtual VOID Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};