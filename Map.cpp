#include "Map.h"

Map::Map()
{
}

void Map::Load(int ID)
{
	string filename = ""; filename = "Map/" + to_string(ID) + ".txt";
	cout << filename << endl;
	ifstream file(filename.c_str(), std::ios::in);

	string ts;
	file >> _layers >> _w >> _h;
	file >> ts;
	filename = "Tilesets/" + ts + ".png";
	_tileset.Load(filename.c_str());

	// Collision
	_collision = new int*[_w];
	for (int i = 0; i < _w; i++)
		_collision[i] = new int[_h];

	for (int i = 0; i < _h; i++)
	{
		for (int j = 0; j < _w; j++) 
			file >> _collision[j][i];
	}

	// Tilemapping
	_map = new Tile**[_layers];
	for (int j = 0; j < _layers; j++)
	{
		_map[j] = new Tile*[_w];
		for (int i = 0; i < _w; i++)
			_map[j][i] = new Tile[_h];
	}

	for (int l = 0; l < _layers; l++)
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int ID;
				file >> ID;
				_map[l][j][i].Load(j, i, ID);
			}
		}
	}

	char buf;

	file >> buf;
	while (buf == '#')
	{
		string type;
		file >> type;

		if (type == "TP")
		{
			int x, y, dx, dy, ID;
			bool t;
			file >> ID >> x >> y >> dx >> dy >> t;

			_events.push_back(new Teleport_E(ID, x, y, dx, dy, t));
		}
		if (type == "ANIM_TILE")
		{
			int x, y, ID, ev_type, nb_frames, *tiles, dir[4];
			float *delays;
			file >> x >> y >> nb_frames;
			tiles = new int[nb_frames];
			delays = new float[nb_frames];
			for (int i = 0; i < nb_frames; i++)
				file >> tiles[i] >> delays[i];

			file >> ev_type;
			if (ev_type == Animated_Tile_E::POS || ev_type == Animated_Tile_E::GOING)
			{
				int ex, ey;
				file >> ex >> ey;
				for (int i = 0; i < 4; i++) file >> dir[i];
				_events.push_back(new Animated_Tile_E(x, y, nb_frames, tiles, delays, ex, ey, (ev_type == Animated_Tile_E::POS), dir));
			}


		}
		file >> buf;
	}

	file.close();
}

void Map::GoTo(int ID)
{
	// Release Memory
	//SDL_DestroyTexture(_tileset);

	for (int i = 0; i < _w; i++) free(_collision[i]);
	free(_collision);
		
	for (int j = 0; j < _layers; j++)
	{
		for (int i = 0; i < _w; i++) free(_map[j][i]);
		free(_map[j]);
	}
	free(_map);

	for (int i = 0; i < _events.size(); i++) delete _events[i];
	_events.clear();


	// Load new map
	string filename = ""; filename = "Map/" + to_string(ID) + ".txt";
	cout << filename << endl;
	ifstream file(filename.c_str(), std::ios::in);

	string ts;
	file >> _layers >> _w >> _h;
	file >> ts;
	filename = "Tilesets/" + ts + ".png";
	_tileset.Load(filename.c_str());

	// Collision
	_collision = new int*[_w];
	for (int i = 0; i < _w; i++)
		_collision[i] = new int[_h];

	for (int i = 0; i < _h; i++)
	{
		for (int j = 0; j < _w; j++)
			file >> _collision[j][i];
	}

	// Tilemapping
	_map = new Tile**[_layers];
	for (int j = 0; j < _layers; j++)
	{
		_map[j] = new Tile*[_w];
		for (int i = 0; i < _w; i++)
			_map[j][i] = new Tile[_h];
	}

	for (int l = 0; l < _layers; l++)
	{
		for (int i = 0; i < _h; i++)
		{
			for (int j = 0; j < _w; j++)
			{
				int ID;
				file >> ID;
				_map[l][j][i].Load(j, i, ID);
			}
		}
	}

	char buf;

	file >> buf;
	while (buf == '#')
	{
		string type;
		file >> type;

		if (type == "TP")
		{
			int x, y, dx, dy, ID;
			bool t;
			file >> ID >> x >> y >> dx >> dy >> t;

			_events.push_back(new Teleport_E(ID, x, y, dx, dy, t));
		}
		if (type == "ANIM_TILE")
		{
			int x, y, ID, ev_type, nb_frames, *tiles, dir[4];
			float *delays;
			file >> x >> y >> nb_frames;
			tiles = new int[nb_frames];
			delays = new float[nb_frames];
			for (int i = 0; i < nb_frames; i++)
				file >> tiles[i] >> delays[i];

			file >> ev_type;
			if (ev_type == Animated_Tile_E::POS || ev_type == Animated_Tile_E::GOING)
			{
				int ex, ey;
				file >> ex >> ey;
				
				for (int i = 0; i < 4; i++) file >> dir[i];
				_events.push_back(new Animated_Tile_E(x, y, nb_frames, tiles, delays, ex, ey, (ev_type == Animated_Tile_E::POS), dir));
			}
			

		}
		file >> buf;
	}
	file.close();
}

void Map::BindPlayer(Player * p)
{
	_player = p;
}

void Map::Update()
{
	if (_player != nullptr)
	{
		_scroll_x = _player->GetPosX() - 540;
		_scroll_y = _player->GetPosY() - 360;
	
		if (_scroll_x < 0) _scroll_x = 0;
		if (_scroll_x > _w * 32 - 1080) _scroll_x = _w * 32 - 1080;
		if (_scroll_y < 0) _scroll_y = 0;
		if (_scroll_y > _h * 32 - 720) _scroll_y = _h * 32 - 720;

		int x = _player->GetX(), y = _player->GetY() + 1;

		// Updating events
		for (int i = 0; i < _events.size(); i++)
		{
			if (string(_events[i]->GetType()) == "Animated_Tile")
			{
				Animated_Tile_E* buf = (Animated_Tile_E*)_events[i];
				buf->Update(_player);
				if (buf->isActivated())
				{
					_map[1][buf->GetX()][buf->GetY() + 1].SetID(buf->GetFrame());
					
				}
			}
			else if (string(_events[i]->GetType()) == "Teleport")
			{
				Teleport_E* buf = (Teleport_E*)_events[i];
				buf->Update(_player);
				if(buf->isDone()) GoTo(buf->GetID());
			}
		}
	}
}

void Map::Display()
{
	int tw = _tileset.GetW(), th = _tileset.GetH();
	for (int l = 0; l < _layers; l++)
	{
		glEnable(GL_DEPTH);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4d(1, 1, 1, 1);

		glBindTexture(GL_TEXTURE_2D, _tileset.GetTex());

		for (int i = 0; i < _w; i++)
		{
			for (int j = 0; j < _h; j++)
			{
				if (_map[l][i][j].GetID() != 20)
					_map[l][i][j].Display(tw, th);
			}
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		if (l == 1 && _player != nullptr) _player->Display(_scroll_x, _scroll_y);
	}
}

void Map::Destroy()
{
	for (int i = 0; i < _w; i++) delete _collision[i];
	delete _collision;

	for (int j = 0; j < _layers; j++)
	{
		for (int i = 0; i < _w; i++)
			delete _map[j][i];
		delete _map[j];
	}
	delete _map;

	_events.clear();
}

int Map::GetW()
{
	return _w;
}

int Map::GetH()
{
	return _h;
}

void Map::MovePlayer(int speed, int dir)
{
	bool itsok = true, jump = false;
	int x = _player->GetX(), y = _player->GetY() + 1;

	if (!_player->isLocked())
	{
		if (dir == 0 && (y >= _h || _collision[x][y + 1] != 0)) itsok = false;
		if (dir == 1 && (y < 1   || _collision[x][y - 1] != 0)) itsok = false;
		if (dir == 2 && (x < 0   || _collision[x - 1][y] != 0)) itsok = false;
		if (dir == 3 && (x >= _w || _collision[x + 1][y] != 0)) itsok = false;

		if (dir == 0 && y < _h && _collision[x][y + 1] == 2 + dir) itsok = jump = true;
		if (dir == 1 && y > 1 && _collision[x][y - 1] == 2 + dir) itsok = jump = true;
		if (dir == 2 && x > 0 && _collision[x - 1][y] == 2 + dir) itsok = jump = true;
		if (dir == 3 && x < _w && _collision[x + 1][y] == 2 + dir) itsok = jump = true;
		
		
		if (itsok)
		{
			if (jump) _player->Jump(dir);
			else if (speed == 0) _player->Walk(dir);
			else if (speed == 1) _player->Run(dir);
		}
		else _player->Turn(dir);
	}
	
}

