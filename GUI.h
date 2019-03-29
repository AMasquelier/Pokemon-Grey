#pragma once
#include "Basics.h"
#include "Graphics.h"
#include "Inputs.h"
#include "Character.h"
#include "Dialogue.h"

class MainInfoGUI
{
	public:
		static void Init();
		static void Destroy();
		static void Display();
		static void Update();

	private:
		static Bitmap _menu, _playerName, _money, _badges, _dex, _playTime;
		static Bitmap _time;
		static Bitmap _team_status;
		static Clock _refresh_time;

};

typedef struct DChoice
{
	vector<string> choices;
	int pos;
} DChoice;

class DialogueGUI
{
	public:
		static void Init();
		static void Destroy();
		static void Display();
		static void Update();
		static void LoadScript(string ID);

	private:
		static int find_ID(string ID, ifstream *file);

		static Dialogue _dial;
		static Bitmap _Box, _lines[3];
		static Bitmap _ChoiceBox, _cursor, _choices_bmp[3];

		static string _ID;

		static bool _update_text, _choosing;
		static Choice *_choice;
		static int _pos_cursor;
		static bool _stop;
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
		static void CaughtPokemon(Pokemon &pkmn, Player *p);
		static bool Fighting;
		static int actPkmn;
		static int ThrowBall;
		static int lost_tariff[9];

	private:
		static Mix_Chunk *_ball_sound[4], *_launch_ball, *_attack_sound;
		static Chrono input_chrono;
		static int last_input;
		static Bitmap _background, _HpBarTop, _HpBarDown, _baseFront, _baseBack;
		static Bitmap _Battlebox, _actionBox, _cursor, _moveBox, _balls;
		static Bitmap _Attack, _Bag, _PkmnTeam, _Run;
		static int _move_pos, _action_pos;
};