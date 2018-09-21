#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <gl\GLU.h>

#include "Basics.h"

#define GE_WINDOWS_CENTERED SDL_WINDOWPOS_CENTERED

class Color;

Color rgba_color(float r, float g, float b, float a);
Color rgb_color(float r, float g, float b);
class Color
{
public:
	Color();
	Color(float R, float G, float B, float A);
	float r, g, b, a;

};

class Bitmap
{
public:
	void Load(const char *filename);
	void LoadText(TTF_Font *font, const char *text, SDL_Color color);
	void Destroy();
	bool isLoaded();
	GLuint GetTex();
	int GetW();
	int GetH();
	~Bitmap();


private:
	GLuint _texture;
	bool _loaded = false;
	int _w, _h;

};

class Draw
{
public:
	// 2D
	static void Rectangle(double X1, double Y1, double X2, double Y2, Color color);
	static void Circle(double x, double y, double r, int accuracy, Color color);
	static void Line(double X1, double Y1, double X2, double Y2, Color color);
	static void Line(Point2D p1, Point2D p2, Color color);
	static void Text(string text, double size, double x, double y);
	static void BITMAP(double x, double y, Bitmap *bmp);
	static void Rotated_BITMAP(double x, double y, double cx, double cy, float a, Bitmap *bmp);
	static void BITMAP_region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, Bitmap *bmp);
	static void tinted_BITMAP_region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, Color color, Bitmap * bmp);
	static void Rotated_BITMAP_Region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, double cx, double cy, float a, Bitmap * bmp);
	static void tinted_Rotated_BITMAP_Region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, double cx, double cy, float a, Color color, Bitmap * bmp);

	static int d;

};

class Camera
{
public:
	Camera();
	void Perspective();
	void GUI();
	void FlatPerspective();
	void Update();
	void Follow(Point2D *p);
	void GoTo(Point2D p);
	void Rotate(float x, float y, float z);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	void SetFOV(double fov);
	void SetZoom(double zoom);
	void SetPos(double x, double y, double z);
	void SetLimit(double lx, double rx, double uy, double dy);
	double X();
	double Y();

private:
	Point2D _pos;
	Point2D *_follow = nullptr;
	double _zoom = 1;
	double _fov = 36;
	float _a = 0, _b = 0, _c = 0;
	double _x_limit_l = 0, _x_limit_r = 0;
	double _y_limit_u = 0, _y_limit_d = 0;
};