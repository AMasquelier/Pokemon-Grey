#include "Basics.h"

int Display_info::width = 0;
int Display_info::height = 0;
SDL_Renderer *Display_info::renderer;


void Chrono::Begin()
{
	begin = chrono::high_resolution_clock::now();
}

double Chrono::GetDelay()
{
	chrono::duration<double> time_span = chrono::high_resolution_clock::now() - begin;
	return time_span.count();
}


double GE_getTime()
{
	return SDL_GetTicks();
}

void GE_rest(double delay)
{
	SDL_Delay(delay);
}

// Point (2D)
double Distance(Point2D p1, Point2D p2)
{
	return sqrt((p1.X() - p2.X()) * (p1.X() - p2.X()) + (p1.Y() - p2.Y()) * (p1.Y() - p2.Y()));
}

bool isInBox(Point2D p, double x, double y, double w, double h)
{
	return (p.X() > x && p.X() < x + w && p.Y() > y && p.Y() < y + h);
}

Point2D::Point2D()
{
}

Point2D::Point2D(double x, double y)
{
	_x = x; _y = y;
}

void Point2D::Set(double x, double y)
{
	_x = x; _y = y;
}

void Point2D::SetX(double x)
{
	_x = x;
}

void Point2D::SetY(double y)
{
	_y = y;
}

double Point2D::X()
{
	return _x;
}

double Point2D::Y()
{
	return _y;
}



SDL_Texture *Load_SDLTexture(const char *filename)
{
	SDL_Surface *buf = IMG_Load(filename);
	SDL_Texture *ret;

	if (buf)
	{
		ret = SDL_CreateTextureFromSurface(Display_info::renderer, buf);
		SDL_FreeSurface(buf);
		return ret;
	}
	else return nullptr;
}

SDL_Texture * Load_SDLText(TTF_Font * font, const char * txt, SDL_Color color)
{
	SDL_Surface *buf = TTF_RenderText_Blended(font, txt, color);
	SDL_Texture *ret;
	if (buf)
	{
		ret = SDL_CreateTextureFromSurface(Display_info::renderer, buf);
		SDL_FreeSurface(buf);
		return ret;
	}
	else return nullptr;
}
