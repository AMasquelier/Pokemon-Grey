#pragma once
#include "Basics.h"

class Tile
{
	public:
		Tile();
		void Load(double x, double y, int ID);
		void Display(int tex_w, int tex_h);
		int GetID();

	private:
		int _ID;
		SDL_Rect _pos, _tile;

};