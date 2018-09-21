#include "Map.h"

Map::Map()
{
}

void Map::Load(int ID)
{
	string filename = ""; filename = "Map/" + to_string(ID) + ".txt";
	cout << filename << endl;
	ifstream file(filename.c_str(), std::ios::in);

	int w, h;
	file >> _layers >> w >> h;
	_w = w; _h = h;

	_tileset.Load("Tilesets/tileset1.png");

	// Events Localisation
	_events_loc = new int*[_w];
	for (int i = 0; i < _w; i++)
		_events_loc[i] = new int[_h];

	for (int i = 0; i < _h; i++)
	{
		for (int j = 0; j < _w; j++)
			file >> _events_loc[j][i];
	}

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
			int x, y, ID;
			file >> ID >> x >> y;

			_events.push_back(new Teleport_E(ID, x, y));
		}
		file >> buf;
	}

	file.close();
}

void Map::GoTo(int ID)
{
	// Release Memory
	//SDL_DestroyTexture(_tileset);

	for (int i = 0; i < _w; i++) free(_events_loc[i]);
	free(_events_loc);

	for (int i = 0; i < _w; i++) free(_collision[i]);
	free(_collision);
		
	for (int j = 0; j < _layers; j++)
	{
		for (int i = 0; i < _w; i++) free(_map[j][i]);
		free(_map[j]);
	}
	free(_map);

	//for (int i = 0; i < _events.size(); i++) delete _events[i];
	_events.clear();

	// Load new map
	string filename = ""; filename = "Map/" + to_string(ID) + ".txt";
	cout << filename << endl;
	ifstream file(filename.c_str(), std::ios::in);

	int w, h;
	file >> _layers >> w >> h;
	_w = w; _h = h;

	// Events Localisation
	_events_loc = new int*[_w];
	for (int i = 0; i < _w; i++)
		_events_loc[i] = new int[_h];

	for (int i = 0; i < _h; i++)
	{
		for (int j = 0; j < _w; j++)
			file >> _events_loc[j][i];
	}

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
			int x, y, ID;
			file >> ID >> x >> y;

			_events.push_back(new Teleport_E(ID, x, y));
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
	_scroll_x = _player->GetPosX() - 540;
	_scroll_y = _player->GetPosY() - 360;
	if (_scroll_x < 0) _scroll_x = 0;
	if (_scroll_x > _w * 32 - 1080) _scroll_x = _w * 32 - 1080;
	if (_scroll_y < 0) _scroll_y = 0;
	if (_scroll_y > _h * 32 - 720) _scroll_y = _h * 32 - 720;

	int x = _player->GetX(), y = _player->GetY() + 1;

	if (_events_loc[x][y] != 0)
	{
		int e = _events_loc[x][y]-1;
		
		if (e <= _events.size())
		{
			if (string(_events[e]->GetType()) == "Teleport")
			{
				Teleport_E *tp = (Teleport_E *)_events[e];
				GoTo(tp->GetID());
				_player->SetX(tp->GetX());
				_player->SetY(tp->GetY());
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

		if (l == 1) _player->Display(_scroll_x, _scroll_y);
	}
}

void Map::Destroy()
{
	for (int i = 0; i < _w; i++) delete _events_loc[i];
	delete _events_loc;

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
