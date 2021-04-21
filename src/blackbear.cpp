#include "blackbear.h"

BlackBear::BlackBear(int id)
	:Bear(id)
{
}

BlackBear::~BlackBear()
{
	cout << "black bear destructor called" << endl;
}