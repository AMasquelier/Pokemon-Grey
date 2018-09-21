#pragma once
# include "Character.h"

class Event
{
	public:
		Event();
		void Activate(Player *player);
		const char*GetType();

	protected:
		char *_type = "Event";
};

class Dialog_E : public Event
{
	public:


	private:

};

class Teleport_E : public Event
{
	public:
		Teleport_E();
		Teleport_E(int ID, int x, int y);
		void Load(int ID, int x, int y);
		int GetID();
		int GetX();
		int GetY();

	private:
		int _ID;
		int _x, _y;

};