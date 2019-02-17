#pragma once
#include "Character.h"

enum { I_BALL, I_HEAL };
class Item;

class Item
{
public:
	Item();
	void Load(int ID, string name, int type, string desc[3]);
	virtual bool Effect(Pokemon *target, Character *trainer);
	Item operator=(Item P);
	int GetID();
	int GetType();
	int GetCategory();
	Bitmap *GetNameBMP();
	Bitmap *GetDescBMP();
	string GetName();
	string GetDesc(int i);
	void SetNb(int nb);
	
	

private:
	int _ID;
	int _type, _category;
	int _nb;
	string _name, _desc[3];
	Bitmap _i_name, _i_desc[3];

};

class Item_bag
{
public:
	Item_bag();
	void Load(int ID, string name, int type, string desc[3]);
	void Display(int n);
	Bitmap *GetNbBMP();
	void equal(Item *P);
	int GetID();
	int GetType();
	int GetCategory();
	string GetName();
	string GetDesc(int i);
	void SetNb(int nb);
	int GetNb();

private:
	int _ID;
	int _type, _category;
	int _nb;
	string _name, _desc[3];
	Bitmap _i_nb;
};

class I_Ball : public Item
{
	public:
		I_Ball();
		I_Ball(float p);
		void Load_i(float p);
		virtual bool Effect(Pokemon *target, Character *trainer);
		float GetProbability();

	private:
		float _prob;
};

class I_Heal : public Item
{
	public:
		I_Heal();
		virtual bool Effect(Pokemon *target, Character *trainer);
		I_Heal(int hp, int att, int def, int attsp, int defsp, int speed, int pp, int status);
		void Load_i(int hp, int att, int def, int attsp, int defsp, int speed, int pp, int status);

	private:
		int _hp, _att, _def, _attsp, _defsp, _speed, _pp, _status, _revive = 0;
};

