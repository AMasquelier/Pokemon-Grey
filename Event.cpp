#include "Event.h"

Event::Event()
{
	_type = "Event";
}

void Event::Activate(Player * player)
{
}

const char * Event::GetType()
{
	return _type;
}

// Teleport
Teleport_E::Teleport_E()
{
	_type = "Teleport";
}

Teleport_E::Teleport_E(int ID, int x, int y)
{
	_type = "Teleport";
	_ID = ID;
	_x = x; _y = y;
	cout << x << "  " << y << "  " << ID << endl;

}

void Teleport_E::Load(int ID, int x, int y)
{
	_ID = ID;
	_x = x; _y = y;
}

int Teleport_E::GetID()
{
	return _ID;
}

int Teleport_E::GetX()
{
	return _x;
}

int Teleport_E::GetY()
{
	return _y;
}
