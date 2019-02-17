#pragma once
# include "Character.h"
#include "Tile.h"
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

class Animated_Tile_E : public Event
{
public:
	Animated_Tile_E();
	Animated_Tile_E(int x, int y, int nb_frames, int *tiles, float *delays, int ex, int ey, bool temp, int dir[4]);

	void Update(Player *p);
	int GetX();
	int GetY();
	int GetFrame();
	bool isActivated();

	enum { ALWAYS, POS, GOING };

private:
	int _x, _y;
	int _etype;
	bool _temp = true; 
	int _nb_frames;
	int *_tiles;
	float *_delays;
	int _dir[4] = { 0, 0, 0, 0 };
	int _ex, _ey;

	float _seq = 0;
	int _frame = 0;
	bool _activated = false;

};

class Teleport_E : public Event
{
	public:
		Teleport_E();
		Teleport_E(int ID, int x, int y, int dx, int dy, bool temp);
		void Update(Player *p);
		int GetID();
		int GetX();
		int GetY();
		bool isDone();

		enum { ON, GOING };

	private:
		int _ID;
		int _x, _y;
		int _dx, _dy;
		bool _etype = false;
		bool _done = false;
};