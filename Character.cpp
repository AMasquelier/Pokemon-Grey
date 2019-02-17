#include "Character.h"


Character::Character()
{
}

void Character::SetName(string name)
{
	_name = name;
}

string Character::GetName()
{
	return _name;
}

bool Character::isLocked()
{
	return _lock;
}

int Character::GetDir()
{
	return _dir;
}


// Player
void Player::Load(int x, int y)
{
	if (!_spriteset.isLoaded()) _spriteset.Load("Characters/Red.png");
	_pos.x = x * 32 - 16; _pos.y = y * 32; _pos.w = _pos.h = 64;
	_sprite.x = _sprite.y = 0; _sprite.w = _sprite.h = 32;
	_x = x; _y = y;

	int n;
	ifstream file("Player.txt");
	file >> n; _nb_pkmn = n;

	string name;
	int nAtt, nDef, nAttSp, nDefSp, nSpeed,
		eHP, eAtt, eDef, eAttSp = 0, eDefSp, eSpeed,
		iHP, iAtt, iDef, iAttSp, iDefSp, iSpeed,
		lvl, actHP, Item, sex, Id, xp, nbm, PPM[4], PPa[4], move[4];
	cout << "Loading player..." << endl;
	for (int i = 0; i < n; i++)
	{
		file >> name >> lvl >> xp >> actHP >> Item >> sex >> Id;
		file >> nAtt >> nDef >> nAttSp >> nDefSp >> nSpeed;
		file >> eHP >> eAtt >> eDef >> eAttSp >> eDefSp >> eSpeed;
		file >> iHP >> iAtt >> iDef >> iAttSp >> iDefSp >> iSpeed;
		file >> nbm;

		for (int i = 0; i < 4; i++)
		{
			if (i < nbm)
				file >> move[i] >> PPa[i] >> PPM[i];
			else
			{
				PPM[i] = 0;
				PPa[i] = 0;
				move[i] = 0;
			}
		}
		_pkmns[i].Load(name,
			nAtt, nDef, nAttSp, nDefSp, nSpeed,
			eHP, eAtt, eDef, eAttSp, eDefSp, eSpeed,
			iHP, iAtt, iDef, iAttSp, iDefSp, iSpeed,
			lvl, actHP, Item, sex, Id, xp,
			nbm, move, PPM, PPa);
	}

	file.close();
}

void Player::Display(double scroll_x, double scroll_y)
{
	Draw::BITMAP_region(_pos.x+2, _pos.y , 64, 64, _sprite.x, _sprite.y, 32, 32, &_spriteset);
	_nx = _x; _ny = _y;
}

void Player::Animate()
{
	int x_dir = 0, y_dir = 0;
	if		(_dir == LEFT)	x_dir = -1;
	else if (_dir == RIGHT) x_dir =  1;
	if		(_dir == UP)	y_dir = -1;
	else if (_dir == DOWN)	y_dir =  1;

	if (_x_os == 1) _x_os = 0;
	if (_y_os == 1) _y_os = 0;
	

	switch (_animation)
	{
		case STAY:
			_frame = 0;
			break;
		case WALK:
			_seq += 0.1;
			if (x_dir != 0) _x_os = fmod(_seq, 2) / 2.0;
			if (y_dir != 0) _y_os = fmod(_seq, 2) / 2.0;
			if ((_seq < 2.001 && _seq > 1.999) || (_seq < 4.001 && _seq > 3.999))
			{
				_x_os = _y_os = 0;
				_lock = false;
				_x += x_dir;
				_y += y_dir;
				_animation = STAY;
			}
			if (_seq >= 4) _seq = 0;

			_frame = _seq;
			break;
		case RUN:
			_seq += 0.2;
			if (x_dir != 0) _x_os = fmod(_seq, 2) / 2.0;
			if (y_dir != 0) _y_os = fmod(_seq, 2) / 2.0;
			if ((_seq < 2.002 && _seq > 1.998) || (_seq < 4.002 && _seq > 3.998))
			{
				_x_os = _y_os = 0;
				_lock = false;
				_x += x_dir;
				_y += y_dir;
				_animation = STAY;
			}
			if (_seq >= 4) _seq = 0;

			_frame = 4 + _seq;
			break;
		case JUMP:
			if(_seq > 1) _seq += 0.2;
			else _seq += 0.08;
			if (x_dir != 0) _x_os = _seq / 2.0;
			if (y_dir != 0) _y_os = _seq / 2.0;
			if (_seq >= 3.98)
			{
				_x_os = _y_os = 0;
				_lock = false;
				_x += x_dir * 2;
				_y += y_dir * 2;
				_animation = STAY;
				_seq = 0;
			}

			_frame = 8;
			break;
	}
	_pos.x = (_x + _x_os * x_dir) * 32 - 16; 
	_pos.y = (_y + _y_os * y_dir) * 32;

	_sprite.x = _frame * 32; _sprite.y = _dir * 32;

	center.Set(_pos.x + 32 - Display_info::width / 2, _pos.y + 32 - Display_info::height / 2);
}

void Player::SetNextPos(int x, int y)
{
	_nx = x; _ny = y;
}

int Player::GetNewX()
{
	return _nx;
}

int Player::GetNewY()
{
	return _ny;
}

void Player::SetPos(int x, int y)
{
	_x = x; _y = y;
}

void Player::SetX(int x)
{
	_x = x;
}

void Player::SetY(int y)
{
	_y = y;
}

int Player::GetX()
{
	return _x;
}

int Player::GetY()
{
	return _y;
}

int Player::GetPosX()
{
	return _pos.x;
}

int Player::GetPosY()
{
	return _pos.y;
}

Point2D * Player::GetCenter()
{
	return &center;
}

int Player::GetNbBadges()
{
	return _nb_badges;
}

void Player::Stay()
{
	_nx = -1; _ny = -1;
	_x_os = _y_os = 0;
	_animation = STAY;
}

void Player::Walk(int dir)
{
	if (!_lock)
	{
		_dir = dir;
		_animation = WALK;
		_lock = true;
		if (dir == DOWN) _ny = _y + 1;
		else if (dir == UP) _ny = _y - 1;
		else if (dir == LEFT) _nx = _x - 1;
		else if (dir == RIGHT) _nx = _x + 1;
	}
}

void Player::Run(int dir)
{
	if (!_lock)
	{
		_dir = dir;
		_animation = RUN;
		_lock = true;
		if (dir == DOWN) _ny = _y + 1;
		else if (dir == UP) _ny = _y - 1;
		else if (dir == LEFT) _nx = _x - 1;
		else if (dir == RIGHT) _nx = _x + 1;
	}
}

void Player::Turn(int dir)
{
	_dir = dir;
	_animation = STAY;
}

void Player::Jump(int dir)
{
	if (!_lock)
	{
		_dir = dir;
		_animation = JUMP;
		_lock = true;
		_seq = 0;
		if (dir == DOWN) _ny = _y + 2;
		else if (dir == UP) _ny = _y - 2;
		else if (dir == LEFT) _nx = _x - 2;
		else if (dir == RIGHT) _nx = _x + 2;
	}
}

int Trainer::GetNbPkmn()
{
	return _nb_pkmn;
}

Pokemon * Trainer::GetPkmn(int n)
{
	if (n < _nb_pkmn) return &_pkmns[n];
	else return nullptr;
}
