#pragma once
#include "Basics.h"
#include "Tile.h"
#include "Graphics.h"
#include "Character.h"
#include "Event.h"
#include "Sound.h"

class Map
{
	public:
		Map();
		void Load(int ID);
		void GoTo(int ID);
		void BindPlayer(Player *p);
		void Update();
		void Display();
		void Destroy();
		int GetW();
		int GetH();
		void PlayMusic();
		void Interact();

		void MovePlayer(int speed, int dir);

	private:
		int _ID;
		int _w, _h, _layers;
		double _scroll_x = 0, _scroll_y = 0;
		Player *_player;
		vector<NPC> _npc;
		Tile ***_map;
		int **_collision;
		Bitmap _tileset;
		vector<Event*> _events;
		Music _music;
};