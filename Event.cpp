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

Teleport_E::Teleport_E(int ID, int x, int y, int dx, int dy, bool type)
{
	_type = "Teleport";
	_ID = ID;
	_x = x; _y = y;
	_dx = dx; _dy = dy;
	_etype = type;
}

void Teleport_E::Update(Player * p)
{
	_done = false;
	if (_etype == ON && !p->isLocked())
	{
		if (p->GetX() == _x && p->GetY() == _y)
		{
			p->SetPos(_dx, _dy);
			_done = true;
		}
	}
	else if (_etype == GOING)
	{
		if (p->GetNewX() == _x && p->GetNewY() == _y)
		{
			p->SetPos(_dx, _dy);
			_done = true;
			if (p->GetDir() == 0) p->SetNextPos(_dy + 1, _dx);
			else if (p->GetDir() == 1)  p->SetNextPos(_dy - 1, _dx);
			else if (p->GetDir() == 2)  p->SetNextPos(_dy, _dx - 1);
			else if (p->GetDir() == 3) p->SetNextPos(_dy, _dx + 1);
		}
	}
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

bool Teleport_E::isDone()
{
	return _done;
}


// Animated tile
Animated_Tile_E::Animated_Tile_E()
{
}

Animated_Tile_E::Animated_Tile_E(int x, int y, int nb_frames, int * tiles, float * delays, int ex, int ey, bool temp, int dir[4])
{
	// Temp -> True = After, False = Before
	_type = "Animated_Tile";
	if (temp) _etype = POS;
	else _etype = GOING;
	_x = x; _y = y;
	_nb_frames = nb_frames;
	_tiles = tiles;
	_delays = delays;
	_ex = ex; _ey = ey;
	_frame = _tiles[0];
	for (int i = 0; i < 4; i++) _dir[i] = dir[i];
}

void Animated_Tile_E::Update(Player *p)
{
	if (_etype == GOING && _dir[p->GetDir()] == 1)
	{
		if ((p->GetNewX() == _ex && p->GetNewY() == _ey) || (_seq > 0 && _seq < _nb_frames))
		{
			_activated = true;
			if (_seq < _nb_frames)
			{
				_frame = _tiles[int(_seq)];
				_seq += _delays[int(_seq)];
			}
		}
		else _activated = false;
	}
	else if (_etype == POS && _dir[p->GetDir()] == 1)
	{
		if ((p->GetX() == _ex && p->GetY() == _ey) || (_seq > 0 && _seq < _nb_frames))
		{
			_activated = true;
			if (_seq < _nb_frames)
			{
				_frame = _tiles[int(_seq)];
				_seq += _delays[int(_seq)];
			}
		}
		else _activated = false;
	}
	
}

int Animated_Tile_E::GetX()
{
	return _x;
}

int Animated_Tile_E::GetY()
{
	return _y;
}

int Animated_Tile_E::GetFrame()
{
	return _frame;
}

bool Animated_Tile_E::isActivated()
{
	return _activated;
}
