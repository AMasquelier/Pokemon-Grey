#pragma once

#include "Pokemon.h"
#include "Character.h"
#include "Graphics.h"
#include "Item.h"
#include "Sound.h"

class Player_info
{
	public:
		static void Load();
		static Player *GetPlayer_ptr();
		static Item_bag _inventory[6][64];
		static void AddItem(int ID, int n);
		static void RemoveItem(int ID, int n);
		static void UpdateBag();
		static int _inv_nb[6];

	private:
		static Player _player;
		

};

class Database
{
	public:
		static void Load(); 
		static void Destroy();
		static Pokemon *GetPkmn(int ID);
		static Item *GetItem(int ID);
		static Capacity *GetCapacity(int ID);
		

		static double TypesEfficcacity[17][17];
		static TTF_Font *font, *font1, *font2, *font3, *font4, *font20, *font17;
		static SDL_Color Black, Red, White, Sad_White;
		static Bitmap pkmn_min, pkmn_front, pkmn_back;
		static Bitmap pkmn_sex, pkmn_hpBar, pkmn_xpBar, pkmn_types, pkmn_item;
		static Bitmap items;
		static Mix_Music *_battle_wild;
		static Sound hop_jump, step_out;

	private:
		static vector<Pokemon> _pkmns;
		static vector<Capacity> _capacities;
		static vector<Item *> _items;

};