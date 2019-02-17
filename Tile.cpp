#include "Tile.h"

Tile::Tile()
{
}

void Tile::Load(double x, double y, int ID)
{
	_ID = ID;
	_pos.x = x * 32; _pos.y = y * 32;
	_pos.w = _pos.h = 32;

	_tile.x = ((_ID - 1) % 6) * 16;
	_tile.y = ((_ID - 1) / 6) * 16;
	_tile.w = _tile.h = 16;
}

void Tile::Display(int tex_w, int tex_h)
{
	int sx = ((_ID - 1) % 6) * 16, sy = ((_ID - 1) / 6) * 16;
	//if (_ID == 359 || _ID == 353) cout << sx << "  " << sy << endl;
	glBegin(GL_QUADS);

	glTexCoord2d(double(sx) / tex_w, double(sy) / tex_h);
	glVertex3d(_pos.x, _pos.y, 0);

	glTexCoord2d(double(sx + _tile.w) / tex_w, double(sy) / tex_h);
	glVertex3d(_pos.x + _pos.w, _pos.y, 0);

	glTexCoord2d(double(sx + _tile.w) / tex_w, double(sy + _tile.h) / tex_h);
	glVertex3d(_pos.x + _pos.w, _pos.y + _pos.h, 0);

	glTexCoord2d(double(sx) / tex_w, double(sy + _tile.h) / tex_h);
	glVertex3d(_pos.x, _pos.y + _pos.h, 0);
	glEnd();
}

void Tile::SetID(int ID)
{
	_ID = ID;
	_tile.x = ((_ID - 1) % 6) * 16;
	_tile.y = ((_ID - 1) / 6) * 16;
	
}

int Tile::GetID()
{
	return _ID;
}
