#pragma once

#include "animal.h"

/*
idbeastiary 0
*/
class Bear : public Animal
{
public:
	Bear(int id = 0);
	virtual ~Bear();

	virtual VOID Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};