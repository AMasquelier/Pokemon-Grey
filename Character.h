#pragma once
#include "Basics.h"
#include "Pokemon.h"

class Character
{
	public:
		Character();
		void SetName(string name);
		string GetName();
		bool isLocked();
		int GetDir();

	protected:
		enum { STAY, WALK, RUN, JUMP };
		enum { DOWN, UP, LEFT, RIGHT };


		string _name;
		SDL_Rect _pos, _sprite;
		int _x, _y;
		int _animation = STAY, _frame = 0, _dir = 0;
		double _seq = 0, _x_os = 0, _y_os = 0;
		bool _lock = false;
};

class Trainer : public Character
{
	public:
		int GetNbPkmn();
		Pokemon *GetPkmn(int n);

	protected:
		Pokemon _pkmns[6];
		int _nb_pkmn;
};

class Player : public Trainer
{
	public:
		void Load(int x, int y);
		void Display(double scroll_x, double scroll_y);
		void Animate();
		
		void SetNextPos(int x, int y);
		int GetNewX();
		int GetNewY();
		void SetPos(int x, int y);
		void SetX(int x);
		void SetY(int y);
		int GetX();
		int GetY();
		int GetPosX();
		int GetPosY();
		Point2D *GetCenter();
		bool AddToTeam(Pokemon *pkmn);

		int GetNbBadges();

		void Stay();
		void Walk(int dir);
		void Run(int dir);
		void Turn(int dir);
		void Jump(int dir);
		
	private:
		int _nx, _ny;
		int _nb_badges = 0;
		Point2D center;
		Bitmap _spriteset;
};