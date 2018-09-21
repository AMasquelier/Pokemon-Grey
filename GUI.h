#pragma once
#include "Basics.h"
#include "Graphics.h"
#include "Inputs.h"

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
		static void Display();
		static void Update();
		static bool Fighting;

	private:
		static Chrono input_chrono;
		static int last_input;
		static Bitmap _menu, _label, _label_cursor;
};