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

// Dialogue GUI < Work in progress >
Bitmap DialogueGUI::_Box;

string DialogueGUI::_ID;
vector<string> DialogueGUI::_text;
vector<int> DialogueGUI::_data;
vector<DChoice> DialogueGUI::_choices;

void DialogueGUI::Init()
{
	if (!_Box.isLoaded()) _Box.Load("Menu/Dialogue.png");

}

void DialogueGUI::Destroy()
{
}

void DialogueGUI::Display()
{
}

void DialogueGUI::Update()
{
	// Controls

	// Manage script
}

void DialogueGUI::LoadScript(string ID)
{
	ifstream file("Dialogue.txt", std::ios::in);
	cout << "Reading Dialogues file..." << endl;
	int pos = 0;
	string act_line;
	if (file)
	{
		int pos_ID = find_ID(ID, &file);
		if (pos_ID != -1)
		{
			file.seekg(pos_ID);

			file >> act_line;
			while (act_line != "<END>")
			{
				if (act_line == ">")
				{
					string buf;
					file >> buf;
					_text.push_back(buf);
					pos++;
				}
				else if (act_line == "<CHOICE>")
				{
					DChoice buf1;
					string buf2;

					buf1.pos = pos - 1;
					file >> buf2;
					while (buf2 != "</>")
					{
						buf1.choices.push_back(buf2);
						file >> buf2;
					}
					_choices.push_back(buf1);
				}

				file >> act_line;
			}
			file.close();
		}
		else cout << "Missing Dialogues file" << endl;

		cout << _text.size() << " Dialogue lines" << endl;
		for (int i = 0; i < _text.size(); i++)
			cout << i << " > " << _text[i] << endl;

		cout << _choices.size() << " choices to make" << endl;
		for (int i = 0; i < _choices.size(); i++)
		{
			cout << "Choice n." << i + 1 << "(size : " << _choices[i].choices.size() << ", pos : " << _choices[i].pos << ")" << endl;
			for (int j = 0; j < _choices[i].choices.size(); j++)
				cout << "    " << _choices[i].choices[j] << endl;
		}

	}
	else cout << "No " << ID << " found" << endl;
}

int DialogueGUI::find_ID(string ID, ifstream *file)
{
	int beg_pos = file->tellg();
	bool found = false;
	string act_line;

	while (!file->eof() && !found)
	{
		int last_pos = file->tellg();
		(*file) >> act_line;
		if (act_line == "#" + ID)
		{
			(*file) >> act_line;
			found = true;
		}
		return last_pos;
	}
	return -1;
}


// Pokemon team GUI
Bitmap PkmnTeamGUI::_menu, PkmnTeamGUI::_label, PkmnTeamGUI::_label_cursor, PkmnTeamGUI::_choice_box, PkmnTeamGUI::_cursor;
Bitmap PkmnTeamGUI::_resume, PkmnTeamGUI::_capacities, PkmnTeamGUI::_item, PkmnTeamGUI::_move, PkmnTeamGUI::_back, PkmnTeamGUI::_choose;
int PkmnTeamGUI::_pos_pcurs = 0, PkmnTeamGUI::_pos_choice = 0;
int PkmnTeamGUI::last_input = 0;
Chrono PkmnTeamGUI::input_chrono;
bool PkmnTeamGUI::_choice = false;
int PkmnTeamGUI::Using_item = -1, PkmnTeamGUI::_Giving_item = -1, PkmnTeamGUI::Selected_Pkmn = -1;
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
	int Offset = -(FightGUI::Fighting) * 43;
	// Menu
	if (!FightGUI::Fighting || (FightGUI::Fighting && Healing == 0 || _pos_pcurs != FightGUI::actPkmn))
	{
		int nb_pkmn = Player_info::GetPlayer_ptr()->GetNbPkmn();
		Draw::BITMAP_region(0, 43 + Offset, 150, 30 + 70 * nb_pkmn, 0, 0, 150, 30 + 70 * nb_pkmn, &_menu);
		Draw::BITMAP_region(0, 47 + Offset + 70 * nb_pkmn, 150, 50, 0, 550, 150, 50, &_menu);
		Draw::BITMAP(18, 68 + _pos_pcurs * 70 + Offset, &_label_cursor);
		for (int i = 0; i < player->GetNbPkmn(); i++)
		{
			Draw::BITMAP(20, 70 + i * 70 + Offset, &_label);
			player->GetPkmn(i)->DisplayMin(i, Offset);
		}

		if (_choice)
		{
			int nb_choices = 3 + (!FightGUI::Fighting) * 2;
			Draw::BITMAP_region(0, 97 + 70 * nb_pkmn + Offset, 125, 5, 0, 0, 125, 5, &_choice_box);
			Draw::BITMAP_region(0, 102 + 70 * nb_pkmn + Offset, 125, nb_choices * 20, 0, 5, 125, 2, &_choice_box);
			Draw::BITMAP_region(0, 102 + Offset + nb_choices * 20 + 70 * nb_pkmn, 125, 5, 0, 7, 125, 5, &_choice_box);
			//Draw::BITMAP(0, 97 + 70 * nb_pkmn, &_choice_box);

			if (!FightGUI::Fighting)
			{
				Draw::BITMAP(24, 105 + 70 * nb_pkmn + Offset, &_resume);
				Draw::BITMAP(24, 125 + 70 * nb_pkmn + Offset, &_capacities);
				Draw::BITMAP(24, 145 + 70 * nb_pkmn + Offset, &_item);
				Draw::BITMAP(24, 165 + 70 * nb_pkmn + Offset, &_move);
				Draw::BITMAP(24, 185 + 70 * nb_pkmn + Offset, &_back);
			}
			else
			{
				if(player->GetPkmn(_pos_pcurs)->GetActHP() > 0 && _pos_pcurs != FightGUI::actPkmn)
					Draw::BITMAP(24, 105 + 70 * nb_pkmn + Offset, &_choose);
				else
					Draw::tinted_BITMAP(24, 105 + 70 * nb_pkmn + Offset, rgb_color(0.75, 0.75, 0.75), &_choose);
				Draw::BITMAP(24, 125 + 70 * nb_pkmn + Offset, &_resume);
				Draw::BITMAP(24, 145 + 70 * nb_pkmn + Offset, &_back);
			}

			Draw::BITMAP(5, 104 + 70 * nb_pkmn + 20 * _pos_choice + Offset, &_cursor);
		}
	}
	
}

void PkmnTeamGUI::Update()
{
	Player *player = Player_info::GetPlayer_ptr();
	int nb_choices = 3 + (!FightGUI::Fighting) * 2;

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
				// Back
				if ((_pos_choice == 4 && !FightGUI::Fighting) || (_pos_choice == 2 && FightGUI::Fighting)) 
				{ _choice = false; _pos_choice = 0; }
				if (_pos_choice == 0 && FightGUI::Fighting)
				{ 
					if (player->GetPkmn(_pos_pcurs)->GetActHP() > 0)
						Selected_Pkmn = _pos_pcurs;
				}
			}
		}
		if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down))
			&& (last_input != 0 || input_chrono.GetDelay() > 0.15))
		{
			last_input = 0;
			input_chrono.Begin();
			if (_choice && _pos_choice < nb_choices-1) 
				_pos_choice++;
			else if (!_choice && _pos_pcurs < Player_info::GetPlayer_ptr()->GetNbPkmn() - 1)
				_pos_pcurs++;
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
		if (FightGUI::Fighting && _pos_pcurs == FightGUI::actPkmn)
		{
			Main::GUI = 0;
		}
		if (player->GetPkmn(_pos_pcurs)->GetActHP() < Healing && player->GetPkmn(_pos_pcurs)->GetActHP() < player->GetPkmn(_pos_pcurs)->GetHP())
		{
			player->GetPkmn(_pos_pcurs)->SetActHP(player->GetPkmn(_pos_pcurs)->GetActHP() + 2);
		}
		else
		{
			player->GetPkmn(_pos_pcurs)->SetActHP(min(Healing, player->GetPkmn(_pos_pcurs)->GetHP()));
			Healing = 0;
			if (FightGUI::Fighting) Main::GUI = 0;
			else Main::GUI = 2;
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
	_nb_choices[0][0] = 3; _nb_choices[0][1] = 2; 
	_nb_choices[1][0] = 4; _nb_choices[1][1] = 2;

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
	int offset = FightGUI::Fighting * 43;
	Draw::BITMAP_region(0, 43 - offset, 200, 600, 200 * (Player_info::_inv_nb[_pos_category] == 0), 0, 200, 600, &_menu);
	Draw::BITMAP(4 + _pos_category * 24, 50 - offset, &_cat_cursor);
	Draw::BITMAP(4, 50 - offset, &_categories);
	if (Player_info::_inv_nb[_pos_category] > 0)
	{
		Draw::BITMAP(8, 90 + 20 * _pos_cursor[_pos_category] - offset, &_cursor);

		for (int i = 0; i < Player_info::_inv_nb[_pos_category]; i++)
		{
			Draw::BITMAP(24, 91 + 20 * i - offset, Database::GetItem(Player_info::_inventory[_pos_category][i].GetID())->GetNameBMP());
			Draw::BITMAP(140 - Player_info::_inventory[_pos_category][i].GetNbBMP()->GetW(), 91 + 20 * i - offset, Player_info::_inventory[_pos_category][i].GetNbBMP());
		}

		Item *act_choosen_item = Database::GetItem(Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID());
		// Item Desc.
		for (int i = 0; i < 3; i++) Draw::BITMAP(10, 566 + 20 * i - offset, &act_choosen_item->GetDescBMP()[i]);
		Draw::BITMAP_region(12, 505 - offset, 64, 64, (act_choosen_item->GetID() % 16) * 32, (act_choosen_item->GetID() / 16) * 32, 32, 32, &Database::items);
		
		if (_choice)
		{
			int item_type = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetType();
			Draw::BITMAP_region(150, 482 - offset - _nb_choices[item_type][FightGUI::Fighting] * 20, 125, 5, 0, 0, 125, 5, &_choice_box);
			Draw::BITMAP_region(150, 487 - offset - _nb_choices[item_type][FightGUI::Fighting] * 20, 125, _nb_choices[item_type][FightGUI::Fighting] * 20, 0, 5, 125, 2, &_choice_box);
			Draw::BITMAP_region(150, 487 - offset, 125, 5, 0, 7, 125, 5, &_choice_box);

			Draw::BITMAP(155, 489 - _nb_choices[item_type][FightGUI::Fighting] * 20 + _pos_choice * 20 - offset, &_cursor);


			for (int i = 0; i < _nb_choices[item_type][FightGUI::Fighting]; i++)
			{
				Draw::BITMAP(174, 489 - _nb_choices[item_type][FightGUI::Fighting] * 20 + i * 20 - offset, &_choices[_choices_list[item_type][i][FightGUI::Fighting]]);
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
			if (_choices_list[item_type][_pos_choice][FightGUI::Fighting] == 0)
			{
				int choosen_item = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID();
				if (Database::GetItem(choosen_item)->GetCategory() == 2)
				{
					Main::GUI = 0;
					_choice = false;
					FightGUI::ThrowBall = choosen_item;

					Player_info::RemoveItem(choosen_item, 1);
				}
				else if (Database::GetItem(choosen_item)->GetCategory() == 1)
				{
					PkmnTeamGUI::Using_item = choosen_item;

					Main::GUI = 1;
					_choice = false;
				}
			}
			if (_choices_list[item_type][_pos_choice][FightGUI::Fighting] == 2)
			{
				PkmnTeamGUI::_Giving_item = Player_info::_inventory[_pos_category][_pos_cursor[_pos_category]].GetID();
				Main::GUI = 1;
				_choice = false;
			}
			if (_choices_list[item_type][_pos_choice][FightGUI::Fighting] == 3) _choice = false;
		}

		if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down))
			&& _pos_choice < _nb_choices[item_type][FightGUI::Fighting]-1 && (last_input != 0 || input_chrono.GetDelay() > 0.15))
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
Bitmap FightGUI::_background, FightGUI::_HpBarTop, FightGUI::_HpBarDown, FightGUI::_baseFront, FightGUI::_baseBack;
Bitmap FightGUI::_Battlebox, FightGUI::_actionBox, FightGUI::_cursor, FightGUI::_moveBox, FightGUI::_balls;
Bitmap FightGUI::_Attack, FightGUI::_Bag, FightGUI::_PkmnTeam, FightGUI::_Run;
int FightGUI::_action_pos = 0, FightGUI::_move_pos = 0, FightGUI::actPkmn = 0, FightGUI::ThrowBall = -1;
bool FightGUI::Fighting = false;
int FightGUI::lost_tariff[9] = { 8, 16, 24, 36, 48, 64, 80, 100, 120 };
void FightGUI::Init()
{
	_background.Load("Menu/Battle/Battbg.png");
	_HpBarTop.Load("Menu/Battle/HpbarP.png");
	_HpBarDown.Load("Menu/Battle/HpbarA.png");
	_baseFront.Load("Menu/Battle/BattleBFront.png");
	_baseBack.Load("Menu/Battle/BattleBBack.png");
	_Battlebox.Load("Menu/Battle/Battlebox.png");
	_actionBox.Load("Menu/Battle/ActionBar.png");
		_Attack.LoadText(Database::font1, "Attaque", Database::Black);
		_Bag.LoadText(Database::font1, "Sac", Database::Black);
		_PkmnTeam.LoadText(Database::font1, "Pkmn", Database::Black);
		_Run.LoadText(Database::font1, "Fuite", Database::Black);
	_cursor.Load("Menu/Battle/Cursor.png");
	_moveBox.Load("Menu/Battle/MoveBar.png");
	_balls.Load("Menu/Balls.png");
}

void FightGUI::Destroy()
{
}
//
//void FightGUI::Battle(Player *p, Trainer *t)
//{
//	double ActTime = 0, LastTime = 0, frame_rate = 60;
//	bool Keep = true;
//
//	while (Keep && !Main::input.CloseGame)
//	{
//		ActTime = GE_getTime();
//
//		if (ActTime - LastTime > 1000.0 / frame_rate)
//		{
//			// Inputs
//			Main::input.UpdateControllerInputs(true);
//			Main::input.UpdateKeyboardInputs(true);
//
//			// Display
//			Draw::BITMAP(0, 0, &_background);
//			Draw::BITMAP(0, 64, &_HpBarTop);
//			Draw::BITMAP(824, 340, &_HpBarDown);
//			Draw::BITMAP_region(0, 720 - 144 - 63, 512, 64, 0, 0, 512, 64, &_baseFront);
//			Draw::BITMAP_region(720, 200, 256, 128, 0, 0, 256, 128, &_baseBack);
//			Draw::BITMAP(0, 720 - 144, &_Battlebox);
//
//			SDL_GL_SwapWindow(Main::screen);
//			LastTime = ActTime;
//		}
//		else GE_rest(1000.0 / frame_rate - (ActTime - LastTime));
//	}
//}

int Deal_Damage(Pokemon *attacker, Pokemon *target, int move, float *efficiency)
{
	int att_ID = attacker->_moves[move]._ID;

	Capacity *chosen_move = Database::GetCapacity(att_ID);
	int att = 1, pow = chosen_move->_power, def = 1;

	if (chosen_move->_category == 1)
	{
		att = attacker->GetAtt();
		def = target->GetDef();
	}
	else
	{
		att = attacker->GetAttSp();
		def = target->GetDefSp();
	}
	float eff = 1;
	float stab = 1;
	eff *= Database::TypesEfficcacity[chosen_move->_type][target->_type1];
	if (target->_type1 == chosen_move->_type) stab = 1.5;
	if (target->_type2 == -1)
	{
		eff *= Database::TypesEfficcacity[chosen_move->_type][target->_type2];
		if (target ->_type2 == chosen_move->_type) stab = 1.5;
	}
	eff *= stab;
	eff *= randomInt(85, 100) * 0.01;
	int lost_hp = double((double(attacker->GetLvl() * 0.4 + 2) * att * pow) / double(def * 50)) * eff;

	(*efficiency) = eff;
	return lost_hp;
}

enum { ATTACK, HEALING, SWITCH, CAPTURE };
typedef struct battle_action
{
	battle_action(int Type, double v, int tar, float eff, int id)
	{
		type = Type;
		value = v;
		ID = id;
		target = tar;
		efficiency = eff;
	}
	int type, ID;
	double value;
	float efficiency;
	int target;

} battle_action;

void FightGUI::Battle(Player *p, int PkmnID, int lvl, int background)
{
	Fighting = true;
	Pokemon *pkmns[2];
	vector<battle_action> actions;

	// Wild Pokémon generation
	int nbm, m[4], PPM[4], PPa[4];
	nbm = 1;
	for (int i = 0; i < 4; i++)
	{
		if (i < nbm)
		{
			m[i] = 21;
			PPM[i] = 35;
			PPa[i] = 35;
		}
		else
		{
			m[i] = 0;
			PPM[i] = 0;
			PPa[i] = 0;
		}
	}

	Pokemon Pkmn;
	Pkmn.Load(Database::GetPkmn(PkmnID)->GetName(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, lvl, 1000, 0, 0, PkmnID, 0, nbm, m, PPM, PPa);
	
	Bitmap name_w, lvl_w;
	name_w.LoadText(Database::font20, (Database::GetPkmn(Pkmn.GetID())->GetName()).c_str(), Database::Black);
	lvl_w.LoadText(Database::font17, ("N. " + to_string(lvl)).c_str(), Database::Black);
	bool Disp_wild_pkmn = true;
	double ko_animation[2] = { 0, 0 };
	pkmns[1] = &Pkmn;

	// Loading trainer's pokémon
	 
	for (int i = 0; i < p->GetNbPkmn(); i++)
	{
		if (p->GetPkmn(i)->GetActHP() > 0)
		{
			actPkmn = i;
			break;
		}
	}
	Pokemon *act_pkmn = p->GetPkmn(actPkmn);
	
	Bitmap name_t, lvl_t, hp_t, actHp_t;
	name_t.LoadText(Database::font20, act_pkmn->GetName().c_str(), Database::Black);
	lvl_t.LoadText(Database::font17, ("N. " + to_string(act_pkmn->GetLvl())).c_str(), Database::Black);
	hp_t.LoadText(Database::font17, to_string(act_pkmn->GetHP()).c_str(), Database::Black);
	actHp_t.LoadText(Database::font17, to_string(act_pkmn->GetActHP()).c_str(), Database::Black);
	float xpcoeff = double(act_pkmn->GetXP()) / (act_pkmn->xpnextlvl - act_pkmn->xpoldlvl);
	if (act_pkmn->GetLvl() == 100) xpcoeff = 1;
	Bitmap moves_name[4], moves_PP[4];

	for (int i = 0; i < act_pkmn->nbMoves; i++)
	{
		moves_name[i].LoadText(Database::font1, Database::GetCapacity(act_pkmn->_moves[i]._ID)->_name.c_str(), Database::Black);
		moves_PP[i].LoadText(Database::font1, ("PP  " + to_string(act_pkmn->_moves[i]._actPP) + " / " + to_string(act_pkmn->_moves[i]._PP)).c_str(), Database::Black);
	}

	pkmns[0] = act_pkmn;

	Camera cam;
	cam.SetPos(0, 0, 0);
	int pos_anim = 320, pos_anim2 = 320; // For launching animation
	string info[3];
	Bitmap info_bmp[3];
	Chrono info_time;
	int act_turn = 0, turns[2] = { 1, 2 }; // 1 for player, 2 for wild pkmn
	int choice = -1;

	// Attacking variables
	double damage_to[2] = { 0, 0 };
	float dealing_speed = 0;
	int healing = 0;
	double t_act_hp[2] = { 0, 0 };

	// Catching variables
	int ballx = 0, bally = 0, catch_count = 0;
	float ballframe = 0;
	bool capturing = false;
	float a_catch = -1, b_catch = -1;
	bool caught = false, move = false;
	int randoms[4], nb_shake = 0;

	// States
	bool battle = false;
	bool intro = true;
	bool launching_pkmn = false;
	bool disp_info = false;
	bool player_choosing = false;
	bool waiting_pushA = false;
	bool resolve = false;
	bool wild_choose = false;
	double earning_xp = 0, earned_xp = 0;
	bool Loose = false;
	bool resolving_turn = false;

	// Main loop
	double ActTime = 0, LastTime = 0, frame_rate = 90;
	bool Keep = true;

	//Keep = false; caught = true;

	while ((Keep && !Main::input.CloseGame) || (waiting_pushA))
	{
		ActTime = GE_getTime();

		if (ActTime - LastTime > 1000.0 / frame_rate)
		{
		Loop_start:
			// Inputs
			Main::input.UpdateControllerInputs(true);
			Main::input.UpdateKeyboardInputs(true);

			  
			if (intro)
			{
				if (pos_anim == 320)
				{
					info_bmp[0].LoadText(Database::font1, ("Un " + pkmns[1]->GetName() + " sauvage apparaît ! ").c_str(), Database::White);
					waiting_pushA = true;
					info_time.Begin();
				}
				if (pos_anim > 0) pos_anim -= 6;
				if (pos_anim < 0) pos_anim = 0;
				else if (!waiting_pushA || info_time.GetDelay() > 1)
				{
					waiting_pushA = false;
					intro = false;
					launching_pkmn = true;
					pos_anim = 0;
				}
			}
			if (launching_pkmn)
			{
				if (pos_anim2 == 320)
				{
					info_bmp[0].LoadText(Database::font1, (pkmns[0]->GetName() + " GO ! ").c_str(), Database::White);
					disp_info = true;
				}
				if (pos_anim2 > 0) pos_anim2 -= 6;
				else
				{
					waiting_pushA = false;
					intro = false;
					pos_anim2 = 0;
					battle = true;
					player_choosing = true;
					launching_pkmn = false;
					disp_info = false;
				}
				if (pos_anim2 < 0) pos_anim2 = 0;
			}
			if (earning_xp > 0)
			{
				earned_xp += earning_xp / 100.0;
				pkmns[0]->SetXP(pkmns[0]->GetXP() + earning_xp / 100.0);
				if (earned_xp >= earning_xp)
				{
					earning_xp = 0;
					Keep = false;
					waiting_pushA = true;
				}
				xpcoeff = double(act_pkmn->GetXP()) / (act_pkmn->xpnextlvl - act_pkmn->xpoldlvl);
			}

			if (waiting_pushA && Main::input.pushedInput(Main::input.Space))
			{
				waiting_pushA = false;
				disp_info = false;
				goto Loop_start;
			}

			if (Main::GUI == 1) PkmnTeamGUI::Update();
			else if (Main::GUI == 2) BagGUI::Update();

			if (battle)
			{
				if (!resolving_turn)
				{
					// Feedback of the GUI's
					if (Main::GUI == 0)
					{
						if (ThrowBall != -1)
						{
							actions.push_back(battle_action(CAPTURE, 0, -1, 1, ThrowBall));
							player_choosing = false;
							wild_choose = true;
							choice = -1;
							Main::GUI = 3;
						}
						if (PkmnTeamGUI::Healing != 0)
						{
							actions.push_back(battle_action(HEALING, PkmnTeamGUI::Healing, 0, 1, 0));
							PkmnTeamGUI::Healing = 0;
							player_choosing = false;
							wild_choose = true;
							choice = -1;
						}
					}

					if (player_choosing && Main::GUI != 2 && Main::GUI != 1)
					{
						// Action choice
						if (choice == -1)
						{
							if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down)) && _action_pos < 2)
								_action_pos += 2;
							if ((Main::input.GetInput(Main::input.Z) || Main::input.GetInput(Main::input.Up)) && _action_pos > 1)
								_action_pos -= 2;
							if ((Main::input.GetInput(Main::input.D) || Main::input.GetInput(Main::input.Right)) && _action_pos % 2 == 0)
								_action_pos += 1;
							if ((Main::input.GetInput(Main::input.Q) || Main::input.GetInput(Main::input.Left)) && _action_pos % 2 == 1)
								_action_pos -= 1;

							if (Main::input.pushedInput(Main::input.Space))
							{
								choice = _action_pos;
							}
						}
						// Attack
						else if (choice == 0)
						{
							if ((Main::input.GetInput(Main::input.S) || Main::input.GetInput(Main::input.Down)) && _move_pos / 2 == 0)
								_move_pos += 2;
							if ((Main::input.GetInput(Main::input.Z) || Main::input.GetInput(Main::input.Up)) && _move_pos / 2 == 1)
								_move_pos -= 2;
							if ((Main::input.GetInput(Main::input.D) || Main::input.GetInput(Main::input.Right)) && _move_pos % 2 == 0)
								_move_pos += 1;
							if ((Main::input.GetInput(Main::input.Q) || Main::input.GetInput(Main::input.Left)) && _move_pos % 2 == 1)
								_move_pos -= 1;

							if (_move_pos > act_pkmn->nbMoves - 1) _move_pos = act_pkmn->nbMoves - 1;

							if (Main::input.pushedInput(Main::input.ESCAPE)) choice = -1;

							if (Main::input.pushedInput(Main::input.Space))
							{
								if (act_pkmn->_moves[_move_pos]._actPP > 0)
								{
									float eff = 1;
									int lost_hp = Deal_Damage(pkmns[0], pkmns[1], _move_pos, &eff);

									actions.push_back(battle_action(ATTACK, lost_hp, 1, eff, act_pkmn->_moves[_move_pos]._ID));
									player_choosing = false;
									wild_choose = true;
									choice = -1;
								}
							}
						}
						// Bag
						if (choice == 1)
						{
							Main::GUI = 2;
							choice = -1;
						}
						// Team
						if (choice == 2)
						{
							Main::GUI = 1;
							choice = -1;
						}
						// Run
						if (choice == 1)
						{

						}
					}

					if (wild_choose)
					{
						float eff = 1;
						int wild_move = randomInt(nbm);
						int lost_hp = Deal_Damage(pkmns[1], pkmns[0], wild_move, &eff);

						actions.push_back(battle_action(ATTACK, lost_hp, 0, eff, pkmns[1]->_moves[wild_move]._ID));

						player_choosing = false;
						choice = -1;
						resolve = true;
						wild_choose = false;
					}
				}
				else
				{
					// Resolving actions
					if (!waiting_pushA && resolve && actions.size() > 0)
					{
						if (actions[0].type == ATTACK)
						{
							int target = actions[0].target;
							if (actions[0].value > 0)
							{
								damage_to[target] = actions[0].value;
								info_bmp[0].LoadText(Database::font1, (pkmns[!target]->GetName() + " utilise " + Database::GetCapacity(actions[0].ID)->_name + " !").c_str(), Database::White);
								t_act_hp[target] = pkmns[target]->GetActHP();
								if (actions[0].efficiency > 1.5) dealing_speed = 4;
								else dealing_speed = 1;
								dealing_speed = actions[0].efficiency * actions[0].efficiency;
							}
							else
							{
								info_bmp[0].LoadText(Database::font1, "Mais cela échoue !", Database::White);
								waiting_pushA = true;
							}
							waiting_pushA = true;
							disp_info = true;
							resolve = false;
						}
						if (actions[0].type == HEALING)
						{
							int target = actions[0].target;
							healing = PkmnTeamGUI::Healing;
							info_bmp[0].LoadText(Database::font1, (pkmns[target]->GetName() + " récupère " + to_string(healing - pkmns[target]->GetActHP() + 2) + " PV").c_str(), Database::White);

							info_time.Begin();
							disp_info = true;
							resolve = false;
						}
						if (actions[0].type == CAPTURE)
						{
							resolve = false;
							capturing = true;
							int pcatch_t = Database::GetPkmn(pkmns[1]->GetID())->GetCatch_t();
							a_catch = (1 - 0.6667 * (pkmns[1]->GetActHP() / pkmns[1]->GetHP())) * pcatch_t * ((I_Ball*)Database::GetItem(ThrowBall))->GetProbability();
							
							if (a_catch >= 255)
							{
								nb_shake = 3;
								caught = true;
							}
							else
							{
								b_catch = 65535 * pow(a_catch/255.0, 0.25);
								for (int i = 0; i < 4; i++)
								{
									randoms[i] = randomInt(65535);
									if (randoms[i] <= b_catch) nb_shake++;
								}
								if (nb_shake == 4) nb_shake -= (caught = true);

							}
						}
						actions.erase(actions.begin());
					}
					if (actions.size() == 0 && resolve)
					{
						player_choosing = true;
						resolving_turn = false;
					}

					if (healing > 0)
					{
						PkmnTeamGUI::Healing = 0;
						pkmns[0]->SetActHP(min(pkmns[0]->GetActHP() + 1, pkmns[0]->GetHP()));
						actHp_t.LoadText(Database::font17, to_string(act_pkmn->GetActHP()).c_str(), Database::Black);
						if (pkmns[0]->GetActHP() == healing)
						{
							healing = 0;
							waiting_pushA = true;
							resolve = true;
						}
					}

					for (int i = 0; i < 2; i++)
					{
						if (damage_to[i] > 0)
						{
							//cout << i << "  " << t_act_hp << " : " << pkmns[i]->GetActHP() - dealing_speed << endl;
							pkmns[i]->SetActHP(fmax(t_act_hp[i] - damage_to[i], pkmns[i]->GetActHP() - dealing_speed));
							//cout << i << "  " << t_act_hp << " : " << pkmns[i]->GetActHP() - dealing_speed << endl;
							actHp_t.LoadText(Database::font17, to_string(act_pkmn->GetActHP()).c_str(), Database::Black);

							if (pkmns[i]->GetActHP() == fmax(0, t_act_hp[i] - damage_to[i]))
							{
								info_bmp[0].LoadText(Database::font1, (pkmns[i]->GetName() + " a perdu " + to_string(int(damage_to[i])) + " PV !").c_str(), Database::White);
								damage_to[i] = 0;
								waiting_pushA = true;
								resolve = true;
								info_time.Begin();
								disp_info = true;
							}
						}
					}

					if (capturing)
					{
						// Curve trajectory
						if (ballx < 830) 
						{
							ballx += 10;
							bally = -10 + 0.0016 * pow(ballx - 510, 2);
							ballframe = (ballx / 110.0);
						}
						// Ball opening
						else if (ballframe < 10) 
						{
							ballframe += 0.1 * fmax(0.1, abs(9 - ballframe));
						}
						// Falling
						else if (bally < 220) 
						{
							Disp_wild_pkmn = false;
							bally += 2;
							ballframe = 10;
						}
						// Shakings
						else 
						{
							if (nb_shake > 0)
							{
								if (!move)
								{
									move = true;
									ballframe = 16;
								}
								else
								{
									// 16 - Stand, 17 - Left, 18 - Stand, 19 - Right
									if (ballframe < 17)
										ballframe += 0.018;
									else if (ballframe < 18)
										ballframe += 0.06;
									else if (ballframe < 19)
										ballframe += 0.04;
									else if (ballframe < 19.9)
										ballframe += 0.06;
									else
									{
										move = false;
										nb_shake--;
									}
								}
							}
							else
							{
								if (caught)
								{
									if (ballframe < 21.9) ballframe += 0.05;
									else
									{
										info_bmp[0].LoadText(Database::font1, ("Et hop ! " + pkmns[1]->GetName() + " sauvage est attrapé !").c_str(), Database::White);
										waiting_pushA = true;
										disp_info = true;
										battle = false;
									}
								}
								else
								{
									info_bmp[0].LoadText(Database::font1, (pkmns[1]->GetName() + " s'est échappé !").c_str(), Database::White);
									waiting_pushA = true;
									disp_info = true;
									resolve = true;
									capturing = false;
									ballx = bally = 0;
									ballframe = 0;
									Disp_wild_pkmn = true;
								}
							}
						}

					}
				}
				if (actions.size() >= 2) resolving_turn = true;
			}
			// Checking if they still alive
			if (pkmns[0]->GetActHP() == 0)
			{
				if (ko_animation[0] == 0)
				{
					PkmnTeamGUI::Selected_Pkmn = -1;
					info_bmp[0].LoadText(Database::font1, (pkmns[0]->GetName() + " est KO ! ").c_str(), Database::White);
					battle = false;
					player_choosing = false;
					disp_info = true;
				}
				if (ko_animation[0] < 200)
					ko_animation[0] += 2;
				else
				{
					bool no_pkmn_left = true;
					for (int i = 0; i < p->GetNbPkmn(); i++)
						if (p->GetPkmn(i)->GetActHP() > 0) no_pkmn_left = false;
					
					if (no_pkmn_left)
					{
						if (Loose && !waiting_pushA) Keep = false;
						else
						{
							Loose = true;
							battle = false;

							int lost_money = 0, max_lvl = 0;
							for (int i = 0; i < p->GetNbPkmn(); i++)
								max_lvl = max(max_lvl, p->GetPkmn(i)->GetLvl());

							lost_money = max_lvl * lost_tariff[p->GetNbBadges()];

							info_bmp[0].LoadText(Database::font1, (p->GetName() + " n'a plus de Pokémons en forme !").c_str(), Database::White);
							info_bmp[1].LoadText(Database::font1, ("Et fait tomber " + to_string(lost_money) + "$ dans la précipitation...").c_str(), Database::White);
							waiting_pushA = true;
							disp_info = true;
						}
					}
					else
					{
						Main::GUI = 1;
						battle = false;
						if (PkmnTeamGUI::Selected_Pkmn != -1)
						{
							// Reseting animations
							battle = true;
							launching_pkmn = true;
							ko_animation[0] = 0;
							pos_anim2 = 320;
							Main::GUI = 3;

							// Load new Pkmn
							actPkmn = PkmnTeamGUI::Selected_Pkmn;
							act_pkmn = p->GetPkmn(actPkmn);
							name_t.LoadText(Database::font20, act_pkmn->GetName().c_str(), Database::Black);
							lvl_t.LoadText(Database::font17, ("N. " + to_string(act_pkmn->GetLvl())).c_str(), Database::Black);
							hp_t.LoadText(Database::font17, to_string(act_pkmn->GetHP()).c_str(), Database::Black);
							actHp_t.LoadText(Database::font17, to_string(act_pkmn->GetActHP()).c_str(), Database::Black);
							float xpcoeff = double(act_pkmn->GetXP()) / (act_pkmn->xpnextlvl - act_pkmn->xpoldlvl);
							if (act_pkmn->GetLvl() == 100) xpcoeff = 1;
							Bitmap moves_name[4], moves_PP[4];

							for (int i = 0; i < act_pkmn->nbMoves; i++)
							{
								moves_name[i].LoadText(Database::font1, Database::GetCapacity(act_pkmn->_moves[i]._ID)->_name.c_str(), Database::Black);
								moves_PP[i].LoadText(Database::font1, ("PP  " + to_string(act_pkmn->_moves[i]._actPP) + " / " + to_string(act_pkmn->_moves[i]._PP)).c_str(), Database::Black);
							}

							pkmns[0] = act_pkmn;
						}
					}
				}
			}
			if (pkmns[1]->GetActHP() == 0)
			{
				if (ko_animation[1] == 0)
				{
					info_bmp[0].LoadText(Database::font1, (pkmns[1]->GetName() + " est KO ! ").c_str(), Database::White);
				}
				if (ko_animation[1] < 200)
					ko_animation[1] += 2;
				else
				{
					earning_xp = pkmns[1]->_BasicXP * (pkmns[1]->GetLvl() / 7.0);
					battle = false;
					info_bmp[0].LoadText(Database::font1, (pkmns[0]->GetName() + " gagne " + to_string(int(earning_xp)) + " points EXP.").c_str(), Database::White);
					info_time.Begin();
					disp_info = true;
				}
			}
			if (!battle && caught)
			{
				Keep = 0;
			}

			// Display
			glClear(GL_COLOR_BUFFER_BIT);

			cam.Update();
			cam.Perspective();

			
			Draw::BITMAP(0, 0, &_background);
			Draw::BITMAP_region(0 - pos_anim, 720 - 144 - 63, 512, 64, 0, 0, 512, 64, &_baseFront);
			Draw::BITMAP_region(720 + pos_anim, 200, 256, 128, 0, 0, 256, 128, &_baseBack);
			

			// Battle
				// Trainer's pokemon
			
			Draw::BITMAP_region(160 - pos_anim2, 453 + ko_animation[0] * 2, 160, 160 - ko_animation[0] * 2, 161 * (act_pkmn->GetID() % 5), 81 * (act_pkmn->GetID() / 5), 80, 80 - ko_animation[0], &Database::pkmn_back);
			Draw::BITMAP(824, 340, &_HpBarDown);
			Draw::BITMAP_region(968, 390, 96.0 * (double(act_pkmn->GetActHP()) / act_pkmn->GetHP()), 6, 0, 3 * int(log(double(act_pkmn->GetActHP()) / act_pkmn->GetHP()) / log(0.5)), 96.0 * (double(act_pkmn->GetActHP()) / act_pkmn->GetHP()), 3, &Database::pkmn_hpBar);
			Draw::BITMAP(864, 358, &name_t);
			Draw::BITMAP(1028, 362, &lvl_t);
			Draw::BITMAP(1040 - hp_t.GetW() / 2, 404, &hp_t);
			Draw::BITMAP(978 - actHp_t.GetW() / 2, 404, &actHp_t);
			Draw::BITMAP_region(872, 424, 192.0 * xpcoeff, 4, 0, 0, 192.0 * xpcoeff, 4, &Database::pkmn_xpBar);

				// Wild Pokemon
			if (Disp_wild_pkmn) Draw::BITMAP_region(770 + pos_anim, 116 + ko_animation[1] * 2, 160, 160 - ko_animation[1] *2, 161 * (Pkmn.GetID() % 5), 81 * (Pkmn.GetID() / 5), 80, 80-ko_animation[1], &Database::pkmn_front);
			Draw::BITMAP(0, 64, &_HpBarTop);
			Draw::BITMAP_region(100, 110, 96.0 * (double(Pkmn.GetActHP()) / Pkmn.GetHP()), 6, 0, 3 * int(log(double(Pkmn.GetActHP()) / Pkmn.GetHP()) / log(0.5)), 96.0 * (double(Pkmn.GetActHP()) / Pkmn.GetHP()), 3, &Database::pkmn_hpBar);
			Draw::BITMAP(12, 78, &name_w);
			Draw::BITMAP(166, 80, &lvl_w);
			
			// GUI
			Draw::BITMAP(0, 720 - 144, &_Battlebox);
			
			if (player_choosing && !disp_info)
			{
				Draw::BITMAP(824, 583, &_actionBox);
					Draw::BITMAP(860, 610, &_Attack);
					Draw::BITMAP(985, 610, &_Bag);
					Draw::BITMAP(860, 662, &_PkmnTeam);
					Draw::BITMAP(985, 662, &_Run);
				Draw::BITMAP(838 + (_action_pos % 2) * 125, 609 + (_action_pos / 2) * 52, &_cursor);

				if (choice == 0)
				{
					Draw::BITMAP(40, 583, &_moveBox);
					Draw::BITMAP(50 + (_move_pos % 2) * 260, 609 + (_move_pos / 2) * 52, &_cursor);

					for (int i = 0; i < act_pkmn->nbMoves; i++)
						Draw::BITMAP(80 + (i % 2) * 260, 610 + (i / 2) * 52, &moves_name[i]);

					Draw::BITMAP(660, 600, &moves_PP[_move_pos]);
					int moveType = Database::GetCapacity(act_pkmn->_moves[_move_pos]._ID)->_type;
					Draw::BITMAP_region(685, 670, 64, 32, (moveType % 4) * 64, (moveType / 4) * 32, 64, 32, &Database::pkmn_types);
				}

				if (Main::GUI == 1) PkmnTeamGUI::Display();
				else if (Main::GUI == 2) BagGUI::Display();
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					if(info_bmp[i].isLoaded()) Draw::BITMAP(30, 600 + 25 * i, &info_bmp[i]);
				}
			}

			// Battle Animation
			if (capturing || caught)
			{
				Draw::BITMAP_region(ballx, bally, 32, 52, 16 * int(ballframe), 26 * ThrowBall, 16, 26, &_balls);
			}

			if (Main::GUI == 1) PkmnTeamGUI::Display();
			if (Main::GUI == 2) BagGUI::Display();

			SDL_GL_SwapWindow(Main::screen);
			LastTime = ActTime;
		}
		else GE_rest(1000.0 / frame_rate - (ActTime - LastTime));
	}
	if (caught) CaughtPokemon(Pkmn, p);
	Fighting = false;
	Main::GUI = 0;
}

void FightGUI::CaughtPokemon(Pokemon &pkmn, Player *p)
{
	Camera cam;
	cam.SetPos(0, 0, 0);
	
	Bitmap txt, cbot, cmid, ctop, cursor, yes, no;
	txt.LoadText(Database::font1, ("Donner un surnom au " + pkmn.GetName() + " capturé ? ").c_str(), Database::White);
	cbot.Load("Menu/chboxbot.png");
	cmid.Load("Menu/chboxmid.png");
	ctop.Load("Menu/chboxtop.png");
	cursor.Load("Menu/itemcurs.png");
	yes.LoadText(Database::font4, "Oui", Database::White);
	no.LoadText(Database::font4, "Non", Database::White);

	int pos_curs = 0;
	bool choice = false;

	double ActTime = 0, LastTime = 0, frame_rate = 90;
	bool Keep = true;

	
	while (Keep)
	{
		ActTime = GE_getTime();
		if (ActTime - LastTime > 1000.0 / frame_rate)
		{
			Main::input.UpdateControllerInputs(true);
			Main::input.UpdateKeyboardInputs(true);

			if (!choice)
			{
				if (Main::input.pushedInput(Main::input.Down) || Main::input.pushedInput(Main::input.S))
					pos_curs = 1;
				else if (Main::input.pushedInput(Main::input.Up) || Main::input.pushedInput(Main::input.Z))
					pos_curs = 0;
				if (Main::input.pushedInput(Main::input.Space))
					choice = true;
			}
			if (choice)
			{
				if (pos_curs == 1)
				{
					if (p->AddToTeam(&pkmn)) Keep = false;
					// else put it in a box - TODO
				}
			}
			// Display
			glClear(GL_COLOR_BUFFER_BIT);

			cam.Update();
			cam.Perspective();

			Draw::BITMAP(0, 0, &_background);
			Draw::BITMAP_region(460, 280, 160, 160, 161 * (pkmn.GetID() % 5), 81 * (pkmn.GetID() / 5), 80, 80, &Database::pkmn_front);
			
			Draw::BITMAP(0, 720 - 144, &_Battlebox);
			Draw::BITMAP(20, 600, &txt);

			if (!choice)
			{
				Draw::BITMAP(930, 590, &ctop);
				Draw::BITMAP_region(930, 595, 125, 45, 0, 0, 125, 45, &cmid);
				Draw::BITMAP(930, 640, &cbot);
				Draw::BITMAP(935, 600 + 20 * pos_curs, &cursor);
				Draw::BITMAP(960, 600, &yes);
				Draw::BITMAP(960, 620, &no);
			}

			SDL_GL_SwapWindow(Main::screen);
			LastTime = ActTime;
		}
		else GE_rest(1000.0 / frame_rate - (ActTime - LastTime));
	}
	
}
