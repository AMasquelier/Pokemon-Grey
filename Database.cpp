#include "Database.h"

Player Player_info::_player;
Item_bag Player_info::_inventory[6][64];
int Player_info::_inv_nb[6];
void Player_info::Load()
{
	_player.Load(0, 0);

	for (int i = 0; i < 6; i++)_inv_nb[i] = 0;
	// Inventory
	ifstream invfile("Save/Inventory.txt");
	int nb, i_ID, nb_i;

	invfile >> nb_i;
	for (int i = 0; i < nb_i; i++)
	{
		invfile >> nb >> i_ID;
		int category = Database::GetItem(i_ID)->GetCategory();

		_inventory[category][_inv_nb[category]].equal(Database::GetItem(i_ID));
		_inventory[category][_inv_nb[category]].SetNb(nb);
		_inv_nb[category]++;
	}

	invfile.close();
}

Player * Player_info::GetPlayer_ptr()
{
	return &_player;
}


void Player_info::AddItem(int ID, int n)
{
	int category = Database::GetItem(ID)->GetCategory();
	Database::GetItem(ID);
	bool _found = false;
	for (int i = 0; i < _inv_nb[category]; i++)
	{
		if (_inventory[category][i].GetID() == ID)
		{
			_found = true;
			if(_inventory[category][i].GetNb() < 99)
				_inventory[category][i].SetNb(_inventory[category][i].GetNb() + n);
		}
	}
	if (!_found)
	{
		_inventory[category][_inv_nb[category]].equal(Database::GetItem(ID));
		_inventory[category][_inv_nb[category]].SetNb(n);
		_inv_nb[category]++;
	}
	UpdateBag();
}

void Player_info::RemoveItem(int ID, int n)
{
	int category = Database::GetItem(ID)->GetCategory();

	bool _found = false;
	for (int i = 0; i < _inv_nb[category]; i++)
	{
		if (_inventory[category][i].GetID() == ID)
		{
			_found = true;
			 _inventory[category][i].SetNb(_inventory[category][i].GetNb() - n);
		}
	}
	UpdateBag();
}

bool CompareItem(Item_bag i1, Item_bag i2)
{
	if (i1.GetNb() <= 0) return false;
	else return true;
}

void Player_info::UpdateBag()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < _inv_nb[i]; j++)
		{
			if (_inventory[i][j].GetNb() <= 0)
			{
				for (int k = 0; k < _inv_nb[i] - j-1; k++)
				{
					string Desc[3];
					for (int l = 0; l < 3; l++) Desc[i] = _inventory[i][j + 1].GetDesc(l);
					_inventory[i][j].Load(_inventory[i][j + 1].GetID(), _inventory[i][j + 1].GetName(), _inventory[i][j + 1].GetType(), Desc);
					_inventory[i][j].SetNb(_inventory[i][j + 1].GetNb());
				}
				_inv_nb[i]--;
			}
		}
	}
	
}



// Table des types
double Database::TypesEfficcacity[17][17] = {	
	1,   1,   1,   1,   1, 0.5,   1,   0, 0.5,   1,   1,   1,   1,   1,   1,   1,   1,
	2,   1, 0.5, 0.5,   1,   2, 0.5,   0,   2,   2,   1,   1,   1,   1, 0.5,   2,   1,
	1,   2,   1,   1,   1, 0.5,   2,   1, 0.5,   1,   1,   1,   2, 0.5,   1,   1,   1,
	1,   1,   1, 0.5, 0.5, 0.5,   1, 0.5,   0,   1,   1,   1,   2,   1,   1,   1,   1, 
	1,   1,   0,   2,   1,   2,   1,   1,   2,   1,   2, 0.5, 0.5,   2,   1,   1,   1,
	1, 0.5,   2,   1, 0.5,   1,   2,   1, 0.5,   1,   2,   1,   1,   1,   1,   2,   1, 
	1, 0.5, 0.5, 0.5,   1,   1,   1, 0.5, 0.5,   2, 0.5,   1,   2,   1,   2,   1,   1,
	0,   1,   1,   1,   1,   1,   1,   2,   1, 0.5,   1,   1,   1,   1,   2,   1,   1, 
	1,   1,   1,   1,   1,   2,   1,   1, 0.5,   1, 0.5, 0.5,   1, 0.5,   1,   2,   1,
	1, 0.5,   1,   1,   1,   1,   1,   2,   1, 0.5,   1,   1,   1,   1,   2,   1,   1,
	1,   1,   1,   1,   1, 0.5,   2,   1,   2,   1, 0.5, 0.5,   2,   1,   1,   2, 0.5, 
	1,   1,   1,   1,   2,   2,   1,   1,   1,   1,   2, 0.5, 0.5,   1,   1,   1, 0.5, 
	1,   1, 0.5, 0.5,   2,   2, 0.5,   1, 0.5,   1, 0.5,   2, 0.5,   1,   1,   1, 0.5, 
	1,   1,   2,   1,   1,   1,   1,   1,   1,   1,   1,   2, 0.5, 0.5,   1,   1, 0.5, 
	1,   2,   1,   2,   1,   1,   1,   1, 0.5,   0,   1,   1,   1,   1, 0.5,   1,   1, 
	1,   1,   2,   1,   2,   1,   1,   1, 0.5,   1, 0.5, 0.5,   2,   1,   1, 0.5,   2, 
	1,   1,   1,   1,   1,   1,   1,   1, 0.5,   1,   1,   1,   1,   1,   1,    1,  2};


TTF_Font *Database::font, *Database::font1, *Database::font2, *Database::font3, *Database::font4, *Database::font17, *Database::font20;
SDL_Color Database::Black, Database::Red, Database::White, Database::Sad_White;
vector<Pokemon> Database::_pkmns;
vector<Capacity> Database::_capacities;
vector<Item*> Database::_items;
Bitmap Database::pkmn_min, Database::pkmn_front, Database::pkmn_back;
Bitmap Database::pkmn_sex, Database::pkmn_hpBar, Database::pkmn_xpBar, Database::pkmn_types, Database::pkmn_item;
Bitmap Database::items;
void Database::Load()
{
	// Colors
	Black = { 53, 53, 53 };
	White = { 223, 223, 223 };
	Red = { 243, 140, 140 };
	Sad_White = { 180, 180, 180 };

	// Fonts
	font = TTF_OpenFont("Fonts/PokemonDP.ttf", 12);
	font1 = TTF_OpenFont("Fonts/PokemonDP.ttf", 25);
	font2 = TTF_OpenFont("Fonts/pkmnems.ttf", 10);
	font3 = TTF_OpenFont("Fonts/DS-DIGIT.ttf", 32);
	font4 = TTF_OpenFont("Fonts/PokemonDP.ttf", 16);
	font17 = TTF_OpenFont("Fonts/PokemonDP.ttf", 17);
	font20 = TTF_OpenFont("Fonts/PokemonDP.ttf", 20);

	items.Load("Menu/Items.png");

	// Pokemons textures
	pkmn_min.Load("Menu/pkmnmin.png");
	pkmn_front.Load("Menu/pkmnFront.png");
	pkmn_back.Load("Menu/pkmnBack.png");
	pkmn_sex.Load("Menu/Sex.png");
	pkmn_hpBar.Load("Menu/hpBar.png");
	pkmn_xpBar.Load("Menu/xpbar.png");
	pkmn_item.Load("Menu/Item.png");
	pkmn_types.Load("Menu/Types.png");


	// Items
	string name, desc[3];
	int nb_i, effect_type;

	ifstream ifile("Database/Items.txt");
	
	ifile >> nb_i;
	for (int i = 0; i < nb_i; i++)
	{
		ifile >> name >> desc[0] >> desc[1] >> desc[2] >> effect_type;
		
		if (effect_type == I_BALL)
		{
			float p;
			ifile >> p;
			_items.push_back(new I_Ball(p));
		}
		else if (effect_type == I_HEAL)
		{
			int hp, att, def, attsp, defsp, speed, pp, status;
			ifile >> hp >> att >> def >> attsp >> defsp >> speed >> pp >> status;
			_items.push_back(new I_Heal(hp, att, def, attsp, defsp, speed, pp, status));
		}
		_items[i]->Load(i, name, effect_type, desc);
	}
	ifile.close();

	// Capacities
	ifstream cpfile("Database/Capacities.txt");
	int type = 0, category = 0, target = 0, power = 0, accuracy = 0, PP = 0,
		attlvl = 0, deflvl = 0, spattlvl = 0, spdeflvl = 0, speedlvl = 0, dodgelvl = 0, acclvl = 0,
		attp = 0, defp = 0, spattp = 0, spdefp = 0, speedp = 0, dodgep = 0, accp = 0,
		sleep = 0, psn = 0, para = 0, freeze = 0, burn = 0, attr = 0, confused = 0, curse = 0, scare = 0, trap = 0, vamp = 0, vision = 0,
		delay = 0, fhitsp = 0, recoil = 0, lifesteal = 0;
	bool blocking = 0, cooldown = 0, doubletap = 0, madness = 0;
	

	cpfile.seekg(cpfile.beg);
	_capacities.resize(367);
	for (int i = 0; i < _capacities.size(); i++)
	{
		cpfile >> name >> type >> category >> target >> accuracy >> power >> PP >>
			attlvl >> deflvl >> spattlvl >> spdeflvl >> speedlvl >> dodgelvl >> acclvl >>
			attp >> defp >> spattp >> spdefp >> speedp >> dodgep >> accp >>
			sleep >> psn >> para >> freeze >> burn >> attr >> confused >> curse >> scare >> trap >> vamp >> vision >>
			delay >> fhitsp >> blocking >> cooldown >> recoil >> lifesteal >> doubletap >> madness;
		_capacities[i].Load(i, name, type, category, target, power, accuracy, PP,
			attlvl, deflvl, spattlvl, spdeflvl, speedlvl, dodgelvl, acclvl,
			attp, defp, spattp, spdefp, speedp, dodgep, accp,
			sleep, psn, para, freeze, burn, attr, confused, curse, scare, trap, vamp, vision,
			delay, fhitsp, blocking, cooldown, recoil, lifesteal, doubletap, madness);
	}

	cpfile.close();

	// Pokémons
	ifstream pkmnfile("Database/Pkmns.txt");
	int nbpkmns, hp, att, def, attsp, defsp, speed, xpcourb, type1, type2, BasicXP;
	string pname;
	pkmnfile >> nbpkmns;
	_pkmns.resize(nbpkmns);
	for (int i = 0; i < nbpkmns; i++)
	{
		pkmnfile >> pname >> hp >> att >> def >> attsp >> defsp >> speed >> xpcourb >> type1 >> type2 >> BasicXP;
		_pkmns[i].bLoad(pname, i, hp, att, def, attsp, defsp, speed, xpcourb, type1, type2, BasicXP);
	}
	pkmnfile.close();
}

void Database::Destroy()
{
	TTF_CloseFont(font); TTF_CloseFont(font1); TTF_CloseFont(font2); TTF_CloseFont(font3); TTF_CloseFont(font4);

	for (int i = 0; i < _items.size(); i++) delete _items[i];
}

Pokemon *Database::GetPkmn(int ID)
{
	
	if(ID < _pkmns.size()) return &_pkmns[ID];
	else return &_pkmns[0];
}

Item *Database::GetItem(int ID)
{
	return _items[ID];
}

Capacity * Database::GetCapacity(int ID)
{
	return &_capacities[ID];
}

