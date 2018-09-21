#include "Item.h"
#include "Database.h"
#include "GUI.h"

Item::Item()
{}

bool Item::Effect(Pokemon *target, Character *trainer)
{
	return false;
}

void Item::Load(int ID, string name, int type, string desc[3])
{
	_ID = ID;
	_name = name; _i_name.LoadText(Database::font4, name.c_str(), Database::White);
	_type = type;
	if (_type == 0) _category = 2;
	if (_type == 1) _category = 1;

	for (int i = 0; i < 3; i++)
	{
		_desc[i] = desc[i];
		_i_desc[i].LoadText(Database::font4, desc[i].c_str(), Database::White);
	}
}

Item Item::operator=(Item P)
{
	_name = P.GetName();
	_ID = P.GetID();
	_type = P.GetType();
	for (int i = 0; i < 3; i++)
		_desc[i] = P.GetDesc(i);

	return *this;
}

int Item::GetID()
{
	return _ID;
}

int Item::GetType()
{
	return _type;
}

int Item::GetCategory()
{
	return _category;
}

string Item::GetName()
{
	return _name;
}

string Item::GetDesc(int i)
{
	return _desc[i];
}

void Item::SetNb(int nb)
{
	_nb = nb;
}

void Item::Display(int n)
{
	Draw::BITMAP(24, 91 + 20 * n, &_i_name);
}

void Item::Display_desc()
{
	for (int i = 0; i < 3; i++) Draw::BITMAP(10, 566 + 20 * i, &_i_desc[i]);
	Draw::BITMAP_region(12, 505, 64, 64, (_ID % 16) * 32, (_ID / 16) * 32, 32, 32, &Database::items);
}


Item_bag::Item_bag()
{
}

void Item_bag::Load(int ID, string name, int type, string desc[3])
{
	_ID = ID;
	_name = name;
	_type = type;
	if (_type == 0) _category = 2;
	if (_type == 1) _category = 1;

	for (int i = 0; i < 3; i++)
	{
		_desc[i] = desc[i];
	}
}

bool Item_bag::Effect(Pokemon * target, Character * trainer)
{
	return false;
}

void Item_bag::Display(int n)
{
	Draw::BITMAP(140 - _i_nb.GetW(), 91 + 20 * n, &_i_nb);
}

void Item_bag::equal(Item *P)
{
	_name = P->GetName();
	_ID = P->GetID();
	_type = P->GetType();
	for (int i = 0; i < 3; i++)
		_desc[i] = P->GetDesc(i);
}

int Item_bag::GetID()
{
	return _ID;
}

int Item_bag::GetType()
{
	return _type;
}

int Item_bag::GetCategory()
{
	return _category;
}

string Item_bag::GetName()
{
	return _name;
}

string Item_bag::GetDesc(int i)
{
	return _desc[i];
}

void Item_bag::SetNb(int nb)
{
	_nb = nb;
	_i_nb.Destroy();
	_i_nb.LoadText(Database::font4, (to_string(_nb)+"x").c_str(), Database::White);
}

int Item_bag::GetNb()
{
	return _nb;
}


I_Ball::I_Ball()
{
}

I_Ball::I_Ball(float p)
{
	_prob = p;
}

// Ball
void I_Ball::Load_i(float p)
{
	_prob = p;
}

bool I_Ball::Effect(Pokemon * target, Character * trainer)
{
	return false;
}

float I_Ball::GetProbability()
{
	return _prob;
}


I_Heal::I_Heal()
{
}

bool I_Heal::Effect(Pokemon * target, Character * trainer)
{
	if (target->GetActHP() == target->GetHP() || (target->GetActHP() == 0 && _revive == 0)) return false;
	if (_hp > 0) PkmnTeamGUI::Healing = target->GetActHP() + _hp;
	return true;
}

I_Heal::I_Heal(int hp, int att, int def, int attsp, int defsp, int speed, int pp, int status)
{
	_hp = hp; _att = att; _def = def; _attsp = attsp; _defsp = defsp; _speed = speed; _pp = pp; _status = status;
}

// Heal
void I_Heal::Load_i(int hp, int att, int def, int attsp, int defsp, int speed, int pp, int status)
{
	_hp = hp; _att = att; _def = def; _attsp = attsp; _defsp = defsp; _speed = speed; _pp = pp; _status = status;
}

