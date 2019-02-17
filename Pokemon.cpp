#include "Pokemon.h"
#include "main.h"
#include "Database.h"


// Capacity
Capacity::Capacity()
{}

void Capacity::Load(int ID, string name, int type, int category, int target, int power, int accuracy, int PP,
	int attlvl, int deflvl, int spattlvl, int spdeflvl, int speedlvl, int dodgelvl, int acclvl,
	int attp, int defp, int spattp, int spdefp, int speedp, int dodgep, int accp,
	int sleep, int psn, int para, int freeze, int burn, int attr, int confused, int curse, int scare, int trap, int vamp, int vision,
	int delay, int fhitsp,
	bool Blocking, bool Cooldown, int recoil, int lifesteal, bool DoubleTap, bool Madness)
{
	
	_ID = ID;
	_name = name;
	_type = type;
	_category = category;
	_target = target;
	_power = power;
	_accuracy = accuracy;
	_PP = PP; _actPP = PP;
	_attlvl = attlvl; _deflvl = deflvl; _spattlvl = spattlvl; _spdeflvl = spdeflvl; _speedlvl = speedlvl; _dodgelvl = dodgelvl; _acclvl = acclvl;
	_attp = attp; _defp = defp; _spattp = spattp; _spdefp = spdefp; _speedp = speedp; _dodgep = dodgep; _accp = accp;
	_sleepp = sleep; _poisonp = psn; _parap = para; _freezep = freeze; _burnp = burn; _attrp = attr; _confusedp = confused; _cursep = curse; _scaredp = scare; _trapp = trap; _vampp = vamp; _visionp = vision;
	_delay = delay; _fhitsp = fhitsp;
	_blocking = Blocking; _cooldown = Cooldown; _recoil = recoil; _lifesteal = lifesteal; _DoubleTap = DoubleTap; _Madness = Madness;
}


// Pokemon
Pokemon::Pokemon()
{
}

Pokemon::~Pokemon()
{
}

void Pokemon::Load(string name, int nAtt, int nDef, int nAttSp, int nDefSp, int nSpeed,
	int eHP, int eAtt, int eDef, int eAttSp, int eDefSp, int eSpeed,
	int iHP, int iAtt, int iDef, int iAttSp, int iDefSp, int iSpeed,
	int lvl, int actHP, int Item, int sex, int ID, int xp,
	int nbm, int m[4], int PPM[4], int PPa[4])

{
	*this = (*Database::GetPkmn(ID));
	char buf1[5], buf2[5];
	string b = "/";

	_name = name; _ID = ID; _Item = Item; _Sex = sex; _actHP = actHP, _Lvl = lvl;
	_nAtt = nAtt; _nDef = nDef; _nAttSp = nAttSp; _nDefSp = nDefSp; _nSpeed = nSpeed;
	_eHP = eHP; _eAtt = eAtt; _eDef = eDef; _eAttSp = eAttSp; _eDefSp = eDefSp; _eSpeed = eSpeed;
	_iHP = iHP; _iAtt = iAtt; _iDef = iDef; _iAttSp = iAttSp; _iDefSp = iDefSp; _iSpeed = iSpeed;
	_Xp = xp;
	nbMoves = 0;
	for (int i = 0; i < 4; i++)
	{
		_moves[i]._ID = m[i];
		_moves[i]._actPP = PPa[i];
		_moves[i]._PP = PPM[i];
		if (PPM[i] > 0) nbMoves++;
	}

	_HP = int((_iHP + 2 * Database::GetPkmn(_ID)->GetHP() + int(_eHP / 4))*(double(_Lvl) / 100.0) + 10 + _Lvl);
	_Att = int((2 * Database::GetPkmn(_ID)->GetAtt() + _iAtt + int(_eAtt / 4))*(double(_Lvl) / 100.0)) + 5;
	_Def = int((2 * Database::GetPkmn(_ID)->GetDef() + _iDef + int(_eDef / 4))*(double(_Lvl) / 100)) + 5;
	_AttSp = int((2 * Database::GetPkmn(_ID)->GetAttSp() + _iAttSp + int(_eAttSp / 4))*(double(_Lvl) / 100.0)) + 5;
	_DefSp = int((2 * Database::GetPkmn(_ID)->GetDefSp() + _iDefSp + int(_eDefSp / 4))*(double(_Lvl) / 100)) + 5;
	_Speed = int((_iSpeed + 2 * Database::GetPkmn(_ID)->GetSpeed() + int(_eSpeed / 4))*(double(_Lvl) / 100)) + 5;

	//Combat
	/*_poss_Back.x = (_ID % 5) * 483; _poss_Back.y = (_ID / 5) * 243;
	_posd_Back.h = _posd_Back.w = _poss_Back.h = _poss_Back.w = 240;
	_posd_Back.x = 50; _posd_Back.y = 500;

	_poss_Front.x = (_ID % 5) * 483; _poss_Front.y = (_ID / 5) * 243;
	_poss_Front.h = _poss_Front.w = _posd_Front.h = _posd_Front.w = 240;
	_posd_Front.x = 500; _posd_Front.y = 50;*/
	
	_Name.LoadText(Database::font, _name.c_str(), Database::White);

	_itoa(_actHP, buf1, 10); _itoa(_HP, buf2, 10);
	if (_actHP != 0)	_Hp.LoadText(Database::font2, (buf1 + b + buf2).c_str(), Database::White);
	else				_Hp.LoadText(Database::font2, "K.O.", Database::Red);
	
	_itoa(_Lvl, buf1, 10); b = "N.";
	_lvl.LoadText(Database::font2, (b + buf1).c_str(), Database::White);

	

	long int xplvl1, xplvl2;
	switch (Database::GetPkmn(_ID)->_xpcourb)
	{
		case 0:
			xplvl1 = 0.8 * pow(_Lvl, 3);
			xplvl2 = 0.8 * pow(_Lvl + 1, 3);
			break;
		case 1:
			xplvl1 = pow(_Lvl, 3);
			xplvl2 = pow(_Lvl + 1, 3);
			break;
		case 2:
			xplvl1 = 1.2*pow(_Lvl, 3) - 15 * pow(_Lvl, 2) + 100 * _Lvl - 140;
			xplvl2 = 1.2*pow(_Lvl + 1, 3) - 15 * pow(_Lvl + 1, 2) + 100 * (_Lvl + 1) - 140;
			break;
		case 3:
			xplvl1 = 1.25*pow(_Lvl, 3);
			xplvl2 = 1.25*pow(_Lvl + 1, 3);
			break;
		case 4:

			break;
	}
	xpcoef = double(_Xp) / double(xplvl2 - xplvl1);
	xpoldlvl = xplvl1;
	xpnextlvl = xplvl2;
	if (_actHP > _HP) _actHP = _HP;

}

void Pokemon::DisplayMin(int n, int os)
{	
	Draw::BITMAP_region(0, 40 + os + 70 * n, 68, 68, ((_ID + 1) % 10) * 68, ((_ID + 1) / 10) * 35, 34, 34, &Database::pkmn_min);
	
	Draw::BITMAP(66, 76 + 70 * n + os, &_Name);
	Draw::BITMAP(66, 92 + 70 * n + os, &_lvl);
	Draw::BITMAP(134 - _Hp.GetW(), 116 + 70 * n + os, &_Hp);
	Draw::BITMAP_region(84, 106 + os + 70 * n, 48.0 * (double(_actHP) / _HP), 3, 0, 3 * int(log(double(_actHP) / _HP) / log(0.5)), 48.0 * (double(_actHP) / _HP), 3, &Database::pkmn_hpBar);
	Draw::BITMAP_region(28, 114 + os + 70 * n, 8, 8, 0, 8 * _Sex, 8, 8, &Database::pkmn_sex);

	if(_Item != -1) Draw::BITMAP_region(42, 96 + os + 70 * n, 16, 16, 0, 0, 16, 16, &Database::pkmn_item);
}

void Pokemon::bLoad(string name, int ID, int HP, int Att, int Def, int AttSp, int DefSp, int Speed, int xpcourb, int type1, int type2, int BasicXP)
{
	_catch_t = 45; // TODO
	_name.resize(name.size());
	for (int i = 0; i < name.size(); i++) _name[i] = name[i]; 
	_ID = ID; _HP = HP; _Att = Att; _Def = Def; _AttSp = AttSp; _DefSp = DefSp; _Speed = Speed;	_xpcourb = xpcourb; _type1 = type1; _type2 = type2;
	_BasicXP = BasicXP;
}


void Pokemon::SetActHP(int v)
{
	_actHP = v;
	if (_actHP < 0) _actHP = 0;
	if (_actHP > _HP) _actHP = _HP;
	_Hp.Destroy();
	if (_actHP != 0)	_Hp.LoadText(Database::font2, (to_string(_actHP) + "/" + to_string(_HP)).c_str(), Database::White);
	else				_Hp.LoadText(Database::font2, "K.O.", Database::Red);
}

int Pokemon::GetID() { return _ID; }
int Pokemon::GetHP() { return _HP; }
int Pokemon::GetActHP() { return _actHP; }
int Pokemon::GetAtt() { return _Att; }
int Pokemon::GetDef() { return _Def; }
int Pokemon::GetAttSp() { return _AttSp; }
int Pokemon::GetDefSp() { return _DefSp; }
int Pokemon::GetSpeed() { return _Speed; }
int Pokemon::GetStatus() { return _status; }
void Pokemon::SetStatus(int v) { _status = v; }
int Pokemon::GetCatch_t() { return _catch_t; }
string Pokemon::GetName() { return _name; }
int Pokemon::GetXP() { return _Xp; }
void Pokemon::SetXP(int v)
{
	_Xp = v;
}

int Pokemon::GetLvl()
{
	return _Lvl;
}

int Pokemon::GetItem()
{
	return _Item;
}

void Pokemon::SetItem(int i)
{
	_Item = i;
	cout << "Item  " << i << endl;
}


Pokemon Pokemon::operator=(Pokemon P)
{
	_bname = P.GetName(); _ID = P.GetID(); _bHP = P.GetHP(); _bAtt = P.GetAtt(); _bDef = P.GetDef(); _bAttSp = P.GetAttSp(); _bDefSp = P.GetDefSp(); _bSpeed = P.GetSpeed();
	_actHP = P.GetHP(); _Lvl = 1;
	string b = "/";
	_xpcourb = P._xpcourb;
	char buf1[5], buf2[5];

	_HP = int((_iHP + 2 * Database::GetPkmn(_ID)->GetHP() + int(_eHP / 4))*(double(_Lvl) / 100.0) + 10 + _Lvl);
	_Att = int((2 * Database::GetPkmn(_ID)->GetAtt() + _iAtt + int(_eAtt / 4))*(double(_Lvl) / 100.0)) + 5;
	_Def = int((2 * Database::GetPkmn(_ID)->GetDef() + _iDef + int(_eDef / 4))*(double(_Lvl) / 100)) + 5;
	_AttSp = int((2 * Database::GetPkmn(_ID)->GetAttSp() + _iAttSp + int(_eAttSp / 4))*(double(_Lvl) / 100.0)) + 5;
	_DefSp = int((2 * Database::GetPkmn(_ID)->GetDefSp() + _iDefSp + int(_eDefSp / 4))*(double(_Lvl) / 100)) + 5;
	_Speed = int((_iSpeed + 2 * Database::GetPkmn(_ID)->GetSpeed() + int(_eSpeed / 4))*(double(_Lvl) / 100)) + 5;

	_BasicXP = P._BasicXP;
	_type1 = P._type1; _type2 = P._type2;

	//Combat
	_Name.LoadText(Database::font, _bname.c_str(), Database::White);

	_itoa(_actHP, buf1, 10); _itoa(_HP, buf2, 10);
	if (_actHP != 0)	_Hp.LoadText(Database::font2, (buf1 + b + buf2).c_str(), Database::White);
	else				_Hp.LoadText(Database::font2, "K.O.", Database::Red);


	_itoa(_Lvl, buf1, 10); b = "N.";
	_lvl.LoadText(Database::font2, (b + buf1).c_str(), Database::White);



	long int xplvl1, xplvl2;
	switch (Database::GetPkmn(_ID)->_xpcourb)
	{
	case 0:
		xplvl1 = 0.8 * pow(_Lvl, 3);
		xplvl2 = 0.8 * pow(_Lvl + 1, 3);
		break;
	case 1:
		xplvl1 = pow(_Lvl, 3);
		xplvl2 = pow(_Lvl + 1, 3);
		break;
	case 2:
		xplvl1 = 1.2*pow(_Lvl, 3) - 15 * pow(_Lvl, 2) + 100 * _Lvl - 140;
		xplvl2 = 1.2*pow(_Lvl + 1, 3) - 15 * pow(_Lvl + 1, 2) + 100 * (_Lvl + 1) - 140;
		break;
	case 3:
		xplvl1 = 1.25*pow(_Lvl, 3);
		xplvl2 = 1.25*pow(_Lvl + 1, 3);
		break;
	case 4:

		break;
	}
	xpcoef = double(_Xp) / double(xplvl2 - xplvl1);
	xpoldlvl = xplvl1;
	xpnextlvl = xplvl2;
	if (_actHP > _HP) _actHP = _HP;

	return *this;
}