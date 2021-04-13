#pragma once

#include "corporeal.h"

class Animal : public Corporeal
{
public:
	Animal(int id);
	virtual ~Animal();

	virtual VOID Eat() const;
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);

	Animal operator +(Animal const &cAnimalInstance);

	friend ostream& operator <<(ostream &ostreamInstance, const Animal &animalInstance);

	template <typename T>
	void DisplayInput(T inputArg);
};

template <typename T>
void Animal::DisplayInput(T inputArg)
{
	cout << inputArg << endl;
}