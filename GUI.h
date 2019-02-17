#pragma once
#include "Basics.h"
#include "Graphics.h"
#include "Inputs.h"
#include "Character.h"

class MainInfoGUI
{
	public:
		static void Init();
		static void Destroy();
		static void Display();
		static void Update();

	private:
		static Bitmap _menu, _playerName, _money, _badges, _dex, _playTime;
		static Bitmap _team_status;

};


class PkmnTeamGUI
{
	public:
		static void Init();
		static void Destroy();
		static void Display();
		static void Update();
		static int Using_item, _Giving_item;
		static int Healing;
		static int Selected_Pkmn;
	private:
		static Chrono input_chrono;
		static int last_input;
		static int _pos_pcurs, _pos_choice;
		static bool _choice;
		
		static Bitmap _menu, _label, _label_cursor, _choice_box, _cursor;
		static Bitmap _resume, _capacities, _item, _move, _back, _choose;
};

class BagGUI
{
	public:
		static void Init();
		static void Open();
		static void Destroy();
		static void Display();
		static void Update();

	private:
		static Chrono input_chrono;
		static int last_input;
		static int _pos_cursor[6], _pos_category, _pos_choice;
		static bool _choice;

		static Bitmap _menu, _cursor, _categories, _cat_cursor, _choice_box;
		static Bitmap _choices[4];
		static int _nb_choices[10][2], _choices_list[10][4][2];
	
};

class FightGUI
{
	public:
		static void Init();
		static void Destroy();
		//static void Battle(Player *p, Trainer *t);
		static void Battle(Player *p, int PkmnID, int lvl, int background);
		static void CoughtPokemon(Pokemon &pkmn);
		static bool Fighting;
		static int actPkmn;
		static int ThrowBall;
		static int lost_tariff[9];

	private:
		static Chrono input_chrono;
		static int last_input;
		static Bitmap _background, _HpBarTop, _HpBarDown, _baseFront, _baseBack;
		static Bitmap _Battlebox, _actionBox, _cursor, _moveBox, _balls;
		static Bitmap _Attack, _Bag, _PkmnTeam, _Run;
		static int _move_pos, _action_pos;
};