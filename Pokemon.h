#pragma once
#include "Basics.h"
#include "Graphics.h"
//Capacity

class Capacity
{
public:
	Capacity();
	void Load(int ID, string name, int type, int category, int target, int power, int accuracy, int PP,
		int attlvl, int deflvl, int spattlvl, int spdeflvl, int speedlvl, int dodgelvl, int acclvl,
		int attp, int defp, int spattp, int spdefp, int speedp, int dodgep, int accp,
		int sleep, int psn, int para, int freeze, int burn, int attr, int confused, int curse, int scare, int trap, int vamp, int vision,
		int delay, int fhitsp,
		bool Blocking, bool Cooldown, int recoil, int lifesteal, bool DoubleTap, bool Madness);
	int _type, _category;
	int _target;
	int _power;
	int _accuracy;
	int _PP, _actPP;
	int _attlvl, _deflvl, _spattlvl, _spdeflvl, _speedlvl, _dodgelvl, _acclvl;
	int _attp, _defp, _spattp, _spdefp, _speedp, _dodgep, _accp;
	int _sleepp, _poisonp, _parap, _freezep, _burnp, _attrp, _confusedp, _cursep, _scaredp, _trapp, _vampp, _visionp;
	int _delay, _fhitsp;
	int _recoil, _lifesteal;
	bool _blocking, _cooldown, _DoubleTap, _Madness;
	int _ID;
	string _name;
};

//Pokémon

class Pokemon
{
private:
	string _bname, _name;

	//Stats
	int _bHP, _bAtt, _bDef, _bAttSp, _bDefSp, _bSpeed;	//Base
	int _nAtt, _nDef, _nAttSp, _nDefSp, _nSpeed;		//Nature

	int _HP, _Att, _Def, _AttSp, _DefSp, _Speed;
	int _Xp;
	int _catch_t;

	int _actHP;

	int _MinAnim = 0, _FrontAnim = 0, _BackAnim = 0;
	int _status = 0;

	Bitmap _Hp, _HpBar, _Name, _lvl;

	int _MinI = 0;

public:
	Pokemon();
	~Pokemon();
	void bLoad(string name, int ID, int HP, int Att, int Def, int AttSp, int DefSp, int Speed, int xpcourb, int type1, int type2, int BasicXP);
	void Load(string name, int nAtt, int nDef, int nAttSp, int nDefSp, int nSpeed,
		int eHP, int eAtt, int eDef, int eAttSp, int eDefSp, int eSpeed,
		int iHP, int iAtt, int iDef, int iAttSp, int iDefSp, int iSpeed,
		int lvl, int actHP, int Item, int sex, int ID, int xp,
		int nbm, int m[4], int PPM[4], int PPa[4]);
	void ReloadMin();
	void DisplayMin(int n, int os);
	void DisplayFront();
	void DisplayBack();
	void Resume();
	int GetID();
	int GetHP();
	int GetActHP();
	void SetActHP(int v);
	int GetAtt();
	int GetDef();
	int GetAttSp();
	int GetDefSp();
	int GetSpeed();
	int GetStatus();
	void SetStatus(int v);
	int GetCatch_t();
	int GetXP();
	void SetXP(int v);
	int GetLvl();
	int GetItem();
	void SetItem(int i);
	string GetName();
	int GetSex();

	int _xpcourb;
	double xpcoef;
	int _type1, _type2;
	int _ID, _Sex, _Lvl;
	int xpnextlvl, xpoldlvl;
	int _BasicXP;
	int nbMoves;
	int _eHP, _eAtt, _eDef, _eAttSp, _eDefSp, _eSpeed;	//EV
	int _iHP, _iAtt, _iDef, _iAttSp, _iDefSp, _iSpeed;	//IV
	int _Item;
	enum Status { Sleep, Poison, Para, Freeze, Burn, Attr, Confused, Curse, Scared, Trap, Vamp, Vision };
	Capacity _moves[4];
	Pokemon Pokemon::operator=(Pokemon P);
};