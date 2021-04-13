#pragma once

#include "entity.h"

class Incorporeal : Entity
{
public:
	Incorporeal(int id);
	virtual ~Incorporeal();

	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
};
