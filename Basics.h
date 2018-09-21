#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <gl\GLU.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>
#include <fstream>
#include <random>
#include <string>

#define PI 3.14159265
using namespace std;

class Display_info
{
	public:
		static int width, height;
		static SDL_Renderer *renderer;
};

class Chrono
{
public:
	void Begin();
	double GetDelay();

private:
	chrono::time_point<chrono::steady_clock> begin;
};

double GE_getTime();
void GE_rest(double delay);

namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int randomInt(int min, int max)
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	bool randomBool(double probability = 0.5)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}

class Point2D
{
	public:
		Point2D();
		Point2D(double x, double y);
		void Set(double x, double y);
		void SetX(double x);
		void SetY(double y);
		double X();
		double Y();

	private:
		double _x, _y;
};


SDL_Texture *Load_SDLTexture(const char *filename);

SDL_Texture *Load_SDLText(TTF_Font *font, const char *txt, SDL_Color color);