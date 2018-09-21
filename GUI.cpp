#include "GUI.h"
#include "main.h"
#include "Database.h"

Bitmap MainInfoGUI::_menu;
Bitmap MainInfoGUI::_playerName;
Bitmap MainInfoGUI::_money;
Bitmap MainInfoGUI::_badges;
Bitmap MainInfoGUI::_dex;
Bitmap MainInfoGUI::_playTime;
Bitmap MainInfoGUI::_team_status;
void MainInfoGUI::Init()
{
	_menu.Load("Menu/Menu.png");
	_team_status.Load("Menu/pkbl.png");

	if (!Database::font) cout << TTF_GetError() << endl;
	else
	{
		_playerName.LoadText(Database::font4, "Red", { 220, 220, 220 });
		_money.LoadText(Database::font, "Money : 5555 $", { 220, 220, 220 });
	}
	
}

void MainInfoGUI::Destroy()
{
}

void MainInfoGUI::Display()
{
	Player *player = Player_info::GetPlayer_ptr();

	Draw::BITMAP(0, 0, &_menu);

	// Player Name
	Draw::BITMAP(650, 4, &_playerName);
	// Money
	Draw::BITMAP(650, 24, &_money);

	// Team status
	for (int i = 0; i < 6; i++)
	{
		int sx = 70 + 22 * i, sy;
		if (i >= player->GetNbPkmn()) sy = 60;
		else if (player->GetPkmn(i)->GetActHP() == 0) sy = 40;
		else if (player->GetPkmn(i)->GetStatus() != 0) sy = 20;
		else sy = 0;
		Draw::BITMAP_region(16 + 20 * i, 10, 20, 20, 0, sy, 20, 20, &_team_status);
	}
}

void MainInfoGUI::Update()
{
}

// Pokemon team GUI
Bitmap PkmnTeamGUI::_menu, PkmnTeamGUI::_label, PkmnTeamGUI::_label_cursor, PkmnTeamGUI::_choice_box, PkmnTeamGUI::_cursor;
Bitmap PkmnTeamGUI::_resume, PkmnTeamGUI::_capacities, PkmnTeamGUI::_item, PkmnTeamGUI::_move, PkmnTeamGUI::_back, PkmnTeamGUI::_choose;
int PkmnTeamGUI::_pos_pcurs = 0, PkmnTeamGUI::_pos_choice = 0;
int PkmnTeamGUI::last_input = 0;
Chrono PkmnTeamGUI::input_chrono;
bool PkmnTeamGUI::_choice = false;
int PkmnTeamGUI::Using_item = -1, PkmnTeamGUI::_Giving_item = -1;
int PkmnTeamGUI::Healing = 0;
void PkmnTeamGUI::Init()
{
	_menu.Load("Menu/PkmnMenu.png");
	_label.Load("Menu/Label.png");
	_label_cursor.Load("Menu/Pkmncurs.png");
	_choice_box.Load("Menu/chbox.png");
	_cursor.Load("Menu/itemcurs.png");

	_resume.LoadText(Database::font4, "Resumé", Database::White);
	_capacities.LoadText(Database::font4, "Capacités", Database::White);
	_item.LoadText(Database::font4, "Objet", Database::White);
	_move.LoadText(Database::font4, "Déplacer", Database::White);
	_back.LoadText(Database::font4, "Retour", Database::White);
	_choose.LoadText(Database::font4, "Choisir", Database::White);
}

void PkmnTeamGUI::Destroy()
{
}

void PkmnTeamGUI::Display()
{
	Player *player = Player_info::GetPlayer_ptr();

	// Menu
	int nb_pkmn = Player_info::GetPlayer_ptr()->GetNbPkmn();
	Draw::BITMAP_region(0, 43, 150, 30 + 70 * nb_pkmn, 0, 0, 150, 30 + 70 * nb_pkmn, &_menu);
	Draw::BITMAP_region(0, 47 + 70 * nb_pkmn, 150, 50, 0, 550, 150, 50, &_menu);
	Draw::BITMAP(18, 68 + _pos_pcurs * 70, &_label_cursor);
	for (int i = 0; i < player->GetNbPkmn(); i++)
	{
		Draw::BITMAP(20, 70 + i * 70, &_label);
		player->GetPkmn(i)->DisplayMin(i);
	}
	
	if (_choice)
	{
		Draw::BITMAP_region(0, 97 + 70 * nb_pkmn, 125, 5, 0, 0, 125, 5, &_choice_box);
		Draw::BITMAP_region(0, 102 + 70 * nb_pkmn, 125, 100, 0, 5, 125, 2, &_choice_box);
		Draw::BITMAP_region(0, 102 + 100 + 70 * nb_pkmn, 125, 5, 0, 7, 125, 5, &_choice_box);
		//Draw::BITMAP(0, 97 + 70 * nb_pkmn, &_choice_box);

		Draw::BITMAP(24, 105 + 70 * nb_pkmn, &_resume);
		Draw::BITMAP(24, 125 + 70 * nb_pkmn, &_capacities);
		Draw::BITMAP(24, 145 + 70 * nb_pkmn, &_item);
		Draw::BITMAP(24, 165 + 70 * nb_pkmn, &_move);
		Draw::BITMAP(24, 185 + 70 * nb_pkmn, &_back);

		Draw::BITMAP(5, 104 + 70 * nb_pkmn + 20 * _pos_choice, &_cursor);
	}
}

void PkmnTeamGUI::Update()
{
	Player *player = Player_info::GetPlayer_ptr();

	if (Healing == 0)
	{
		if (Main::input.pushedInput(Main::input.ESCAPE))
		{
			if (_choice) _choice = false;
			else Main::GUI = 0;
		}
		if (!_choice)
		{
			if (Main::input.pushedInput(Main::input.E)) Main::GUI = 2;
		}
		if (Main::input.pushedInput(Main::input.Space))
		{
			if (!_choice)
			{
				if (Using_item != -1)
				{
					if(Database::GetItem(Using_item)->Effect(player->GetPkmn(_pos_pcurs), Player_info::GetPlayer_ptr()))
						Player_info::RemoveItem(Using_item, 1);

					if (!Healing) Main::GUI = 2;
					Using_item = -1;
				}
				else if (_Giving_item != -1)
				{
					Main::GUI = 2;
					if (player->GetPkmn(_pos_pcurs)->GetItem() != -1) Player_info::AddItem(player->GetPkmn(_pos_pcurs)->GetItem(), 1);
					player->GetPkmn(_pos_pcurs)->SetItem(_Giving_item);
					Player_info::RemoveItem(_Giving_item, 1);

					_Giving_item = -1;
				}
				else
				{
					_choice = true;
				}
			}
			else
			{
				if (_pos_choice == 4) { _choice = false; _pos_choice = 0; }
			}
		}
		if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down))
			&& (last_input != 0 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 0;
			input_chrono.Begin();
			if (_choice && _pos_choice < 4) _pos_choice++;
			else if (!_choice && _pos_pcurs < Player_info::GetPlayer_ptr()->GetNbPkmn() - 1) _pos_pcurs++;
		}
		if ((Main::input.GetInput(Main::input.Z) || Main::input.GetInput(Main::input.Up))
			&& (last_input != 1 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 1;
			input_chrono.Begin();
			if (_choice && _pos_choice > 0) _pos_choice--;
			else if (!_choice && _pos_pcurs > 0) _pos_pcurs--;
		}
	}
	else
	{
		if (player->GetPkmn(_pos_pcurs)->GetActHP() < Healing && player->GetPkmn(_pos_pcurs)->GetActHP() < player->GetPkmn(_pos_pcurs)->GetHP())
			player->GetPkmn(_pos_pcurs)->SetActHP(player->GetPkmn(_pos_pcurs)->GetActHP() + 2);
		else
		{
			player->GetPkmn(_pos_pcurs)->SetActHP(min(Healing, player->GetPkmn(_pos_pcurs)->GetHP()));
			Healing = 0;
			Main::GUI = 2;
		}
	}
}

// Bag GUI
Bitmap BagGUI::_menu, BagGUI::_cursor, BagGUI::_categories, BagGUI::_cat_cursor, BagGUI::_choice_box;
Bitmap BagGUI::_choices[4];
int BagGUI::_pos_cursor[6] = { 0, 0, 0, 0, 0, 0 }, BagGUI::_pos_category = 2, BagGUI::_pos_choice = 0;
int BagGUI::last_input = 0;
Chrono BagGUI::input_chrono;
bool BagGUI::_choice = false;
int BagGUI::_nb_choices[10][2], BagGUI::_choices_list[10][4][2];
void BagGUI::Init()
{
	_menu.Load("Menu/BagMenu.png");
	_cursor.Load("Menu/itemcurs.png");
	_categories.Load("Menu/Bag.png");
	_cat_cursor.Load("Menu/bagcurs.png");
	_choice_box.Load("Menu/chbox.png");

	_choices[0].LoadText(Database::font4, "Utiliser", Database::White);
	_choices[1].LoadText(Database::font4, "Jeter", Database::White);
	_choices[2].LoadText(Database::font4, "Donner", Database::White);
	_choices[3].LoadText(Database::font4, "Retour", Database::White);

	//		   [type][inFight]
	_nb_choices[0][0] = 3; _nb_choices[0][1] = 3; 
	_nb_choices[1][0] = 4; _nb_choices[1][1] = 3;

	//			 [type][choice][inFight]
	_choices_list[0][0][0] = 1; _choices_list[0][1][0] = 2; _choices_list[0][2][0] = 3; _choices_list[1][2][0] = -1;
	_choices_list[0][0][1] = 0; _choices_list[0][1][1] = 3; _choices_list[0][2][1] = -1; _choices_list[1][2][0] = -1;

	_choices_list[1][0][0] = 0; _choices_list[1][1][0] = 1; _choices_list[1][2][0] = 2; _choices_list[1][3][0] = 3;
	_choices_list[1][0][1] = 0; _choices_list[1][1][1] = 3; _choices_list[1][2][1] = -1; _choices_list[1][2][1] = -1;
}

void BagGUI::Open()
{
}

void BagGUI::Destroy()
{
}

void BagGUI::Display()
{
	Draw::BITMAP_region(0, 43, 200, 600, 200 * (Player_info::_inv_nb[_pos_category] == 0), 0, 200, 600, &_menu);
	Draw::BITMAP(4 + _pos_category * 24, 50, &_cat_cursor);
	Draw::BITMAP(4, 50, &_categories);
	if (Player_info::_inv_nb[_pos_category] > 0)
	{
		Draw::BITMAP(8, 90 + 20 * _pos_cursor[_pos_category], &_cursor);

		for (int i = 0; i < Player_info::_inv_nb[_pos_category]; i++)
		{
			Database::GetItem(Player_info::_inventory[_pos_category][i].GetID())->Display(i);
			Player_info::_inventory[_pos_category][i].Display(i);
		}

		Database::GetItem(Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID())->Display_desc();
		
		if (_choice)
		{
			int item_type = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetType();
			Draw::BITMAP_region(150, 482 - _nb_choices[item_type][0] * 20, 125, 5, 0, 0, 125, 5, &_choice_box);
			Draw::BITMAP_region(150, 487 - _nb_choices[item_type][0] * 20, 125, _nb_choices[item_type][0] * 20, 0, 5, 125, 2, &_choice_box);
			Draw::BITMAP_region(150, 487, 125, 5, 0, 7, 125, 5, &_choice_box);

			Draw::BITMAP(155, 489 - _nb_choices[item_type][0] * 20 + _pos_choice * 20, &_cursor);


			for (int i = 0; i < _nb_choices[item_type][0]; i++)
			{
				Draw::BITMAP(174, 489 - _nb_choices[item_type][0] * 20 + i * 20, &_choices[_choices_list[item_type][i][0]]);
			}
		}
	}
}

void BagGUI::Update()
{
	if (_choice)
	{
		int item_type = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetType();
		if (Main::input.pushedInput(Main::input.ESCAPE))  _choice = false;

		if (Main::input.pushedInput(Main::input.Space))
		{
			if (_choices_list[item_type][_pos_choice][0] == 0)
			{
				PkmnTeamGUI::Using_item = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID();
				Main::GUI = 1;
				_choice = false;
			}
			if (_choices_list[item_type][_pos_choice][0] == 2)
			{
				PkmnTeamGUI::_Giving_item = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID();
				Main::GUI = 1;
				_choice = false;
			}
			if (_choices_list[item_type][_pos_choice][0] == 3) _choice = false;
		}

		if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down))
			&& _pos_choice < _nb_choices[item_type][0]-1 && (last_input != 0 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 0;
			input_chrono.Begin();
			_pos_choice++;
		}
		if ((Main::input.GetInput(Main::input.Z) || Main::input.GetInput(Main::input.Up))
			&& _pos_choice > 0 && (last_input != 1 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 1;
			input_chrono.Begin();
			_pos_choice--;
		}
	}
	else
	{
		if (Main::input.pushedInput(Main::input.ESCAPE))  Main::GUI = 0;

		if (Main::input.pushedInput(Main::input.T)) Main::GUI = 1;

		if (Main::input.pushedInput(Main::input.Space))
		{
			if (!_choice && Player_info::_inv_nb[_pos_category] > 0) { _choice = true; _pos_choice = 0; }
		}

		if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down))
			&& _pos_cursor[_pos_category] < Player_info::_inv_nb[_pos_category] - 1 && (last_input != 0 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 0;
			input_chrono.Begin();
			_pos_cursor[_pos_category]++;
		}
		if ((Main::input.GetInput(Main::input.Z) || Main::input.GetInput(Main::input.Up))
			&& _pos_cursor[_pos_category] > 0 && (last_input != 1 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 1;
			input_chrono.Begin();
			_pos_cursor[_pos_category]--;
		}
		if ((Main::input.GetInput(Main::input.D) || Main::input.GetInput(Main::input.Right))
			&& _pos_category < 5 && (last_input != 2 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 2;
			input_chrono.Begin();
			_pos_category++;
		}
		if ((Main::input.GetInput(Main::input.Q) || Main::input.GetInput(Main::input.Left))
			&& _pos_category > 0 && (last_input != 3 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 3;
			input_chrono.Begin();
			_pos_category--;
		}
	}

		
}


// Fight GUI
bool FightGUI::Fighting = false;
void FightGUI::Init()
{
}
