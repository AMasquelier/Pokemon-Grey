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
	glBegin(GL_QUADS);

	glTexCoord2d(double(_tile.x) / tex_w, double(_tile.y) / tex_h);
	glVertex3d(_pos.x, _pos.y, 0);

	glTexCoord2d(double(_tile.x + _tile.w) / tex_w, double(_tile.y) / tex_h);
	glVertex3d(_pos.x + _pos.w, _pos.y, 0);

	glTexCoord2d(double(_tile.x + _tile.w) / tex_w, double(_tile.y + _tile.h) / tex_h);
	glVertex3d(_pos.x + _pos.w, _pos.y + _pos.h, 0);

	glTexCoord2d(double(_tile.x) / tex_w, double(_tile.y + _tile.h) / tex_h);
	glVertex3d(_pos.x, _pos.y + _pos.h, 0);
	glEnd();
}

int Tile::GetID()
{
	return _ID;
}
