#include "Graphics.h"

//COLOR
Color rgba_color(float r, float g, float b, float a)
{
	Color c(r, g, b, a);
	return c;
}

Color rgb_color(float r, float g, float b)
{
	Color c(r, g, b, 1);
	return c;
}

Color::Color()
{
	r = g = b = a = 0;
}

Color::Color(float R, float G, float B, float A)
{
	r = R; g = G; b = B; a = A;
}

// Bitmap
void Bitmap::Load(const char *filename)
{
	SDL_Surface *img = IMG_Load(filename);

	if (_loaded) glDeleteTextures(1, &_texture);

	if (img)
	{
		_texture = 0;
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		int nColors;
		GLenum textureFormat;
		nColors = img->format->BytesPerPixel;
		_w = img->w; _h = img->h;

		if (nColors == 4)
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
		}
		else if (nColors == 3) //no alpha channel
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
		}
		else
		{
			cout << nColors << endl;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, textureFormat, GL_UNSIGNED_BYTE, img->pixels);
		_loaded = true;
		SDL_FreeSurface(img);
	}
	else cout << "Can't load " << filename << " : " << IMG_GetError() << endl;
}

void Bitmap::LoadText(TTF_Font * font, const char * text, SDL_Color color)
{
	SDL_Surface *img = TTF_RenderText_Blended(font, text, color);

	if (_loaded) glDeleteTextures(1, &_texture);

	if (img)
	{
		_texture = 0;
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		int nColors;
		GLenum textureFormat;
		nColors = img->format->BytesPerPixel;
		_w = img->w; _h = img->h;
		//contains an alpha channel
		if (nColors == 4)
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
		}
		else if (nColors == 3) //no alpha channel
		{
			if (img->format->Rmask == 0x000000ff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
		}
		else
		{
			cout << nColors << endl;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, textureFormat, GL_UNSIGNED_BYTE, img->pixels);
		_loaded = true;
		SDL_FreeSurface(img);
	}
	else cout << text << "  :  " << TTF_GetError() << endl;
}

void Bitmap::Destroy()
{
	if (_loaded) glDeleteTextures(1, &_texture);
	_loaded = false;
}

bool Bitmap::isLoaded()
{
	return _loaded;
}

GLuint Bitmap::GetTex()
{
	return _texture;
}

int Bitmap::GetW()
{
	return _w;
}

int Bitmap::GetH()
{
	return _h;
}

Bitmap::~Bitmap()
{
	if(_loaded) glDeleteTextures(1, &_texture);
}


// 2D
void Draw::Rectangle(double X1, double Y1, double X2, double Y2, Color color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_QUADS);
	glVertex3f(X1, Y1, 0);
	glVertex3f(X2, Y1, 0);
	glVertex3f(X2, Y2, 0);
	glVertex3f(X1, Y2, 0);
	glEnd();
	glDisable(GL_BLEND);
}

void Draw::Circle(double x, double y, double r, int accuracy, Color color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_POLYGON);
	float a = (2 * 3.1415) / accuracy;
	for (int i = 0; i < accuracy; i++)
	{
		glVertex3f(x + cos(i*a) * r, y + sin(i*a) * r, 0);
		//cout << cos(i*(3.14 / 8)) << endl;
	}
	//cout << endl;
	glEnd();
	glDisable(GL_BLEND);
}

void Draw::Line(double X1, double Y1, double X2, double Y2, Color color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINES);
	glVertex2f(X1, Y1);
	glVertex2f(X2, Y2);
	glEnd();
	glDisable(GL_BLEND);
}

void Draw::Line(Point2D p1, Point2D p2, Color color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(GL_LINES);
	glVertex2f(p1.X(), p1.Y());
	glVertex2f(p2.X(), p2.Y());
	glEnd();
	glDisable(GL_BLEND);
}

void Draw::BITMAP(double x, double y, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());
	glBegin(GL_QUADS);


	glTexCoord2d(0, 0);
	glVertex3d(x, y, 0);

	glTexCoord2d(1, 0);
	glVertex3d(x + bmp->GetW(), y, 0);

	glTexCoord2d(1, 1);
	glVertex3d(x + bmp->GetW(), y + bmp->GetH(), 0);

	glTexCoord2d(0, 1);
	glVertex3d(x, y + bmp->GetH(), 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Draw::Rotated_BITMAP(double x, double y, double cx, double cy, float a, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(1, 1, 1, 1);


	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());
	glBegin(GL_QUADS);

	double dist;
	float a2;
	Point2D p1, p2;

	glTexCoord2d(0, 0);
	dist = sqrt(cx * cx + cy * cy);
	a2 = atan2(-cy, -cx);

	glVertex3d(x + cx + cos(a2 + a) * dist, y + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d(1, 0);
	dist = sqrt((bmp->GetW() - cx)* (bmp->GetW() - cx) + cy * cy);
	a2 = atan2(-cy, bmp->GetW() - cx);

	glVertex3d(x + cx + cos(a2 + a) * dist, y + cy + sin(a2 + a) * dist, 0.1);



	glTexCoord2d(1, 1);
	dist = sqrt((bmp->GetW() - cx)* (bmp->GetW() - cx) + (bmp->GetH() - cy) * (bmp->GetH() - cy));
	a2 = atan2(bmp->GetH() - cy, bmp->GetW() - cx);

	glVertex3d(x + cx + cos(a2 + a) * dist, y + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d(0, 1);
	dist = sqrt(cx * cx + (bmp->GetH() - cy) * (bmp->GetH() - cy));
	a2 = atan2(bmp->GetH() - cy, -cx);

	glVertex3d(x + cx + cos(a2 + a) * dist, y + cy + sin(a2 + a) * dist, 0.1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);


}

void Draw::BITMAP_region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());

	glBegin(GL_QUADS);

	glTexCoord2d(sx / bmp->GetW(), sy / bmp->GetH());
	glVertex3d(dx, dy, 0);

	glTexCoord2d((sx + sw) / bmp->GetW(), sy / bmp->GetH());
	glVertex3d(dx + dw, dy, 0);

	glTexCoord2d((sx + sw) / bmp->GetW(), (sy + sh) / bmp->GetH());
	glVertex3d(dx + dw, dy + dh, 0);

	glTexCoord2d(sx / bmp->GetW(), (sy + sh) / bmp->GetH());
	glVertex3d(dx, dy + dh, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Draw::tinted_BITMAP(double x, double y, Color color, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(color.r, color.g, color.b, color.a);

	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());
	glBegin(GL_QUADS);


	glTexCoord2d(0, 0);
	glVertex3d(x, y, 0);

	glTexCoord2d(1, 0);
	glVertex3d(x + bmp->GetW(), y, 0);

	glTexCoord2d(1, 1);
	glVertex3d(x + bmp->GetW(), y + bmp->GetH(), 0);

	glTexCoord2d(0, 1);
	glVertex3d(x, y + bmp->GetH(), 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Draw::tinted_BITMAP_region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, Color color, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(color.r, color.g, color.b, color.a);

	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());

	glBegin(GL_QUADS);

	glTexCoord2d(sx / bmp->GetW(), sy / bmp->GetH());
	glVertex3d(dx, dy, 0);

	glTexCoord2d((sx + sw) / bmp->GetW(), sy / bmp->GetH());
	glVertex3d(dx + dw, dy, 0);

	glTexCoord2d((sx + sw) / bmp->GetW(), (sy + sh) / bmp->GetH());
	glVertex3d(dx + dw, dy + dh, 0);

	glTexCoord2d(sx / bmp->GetW(), (sy + sh) / bmp->GetH());
	glVertex3d(dx, dy + dh, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Draw::Rotated_BITMAP_Region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, double cx, double cy, float a, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(1, 1, 1, 1);


	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());
	glBegin(GL_QUADS);

	double dist;
	float a2;
	Point2D p1, p2;

	glTexCoord2d(sx / bmp->GetW(), sy / bmp->GetH());
	dist = sqrt(pow(cx, 2) + pow(cy, 2));
	a2 = atan2(-cy, -cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d((sx + sw) / bmp->GetW(), sy / bmp->GetH());
	dist = sqrt(pow(dw - cx, 2) + pow(cy, 2));
	a2 = atan2(-cy, dw - cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);



	glTexCoord2d((sx + sw) / bmp->GetW(), (sy + sh) / bmp->GetH());
	dist = sqrt(pow(dw - cx, 2) + pow(dh - cy, 2));
	a2 = atan2(dh - cy, dw - cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d(sx / bmp->GetW(), (sy + sh) / bmp->GetH());
	dist = sqrt(pow(cx, 2) + pow(dh - cy, 2));
	a2 = atan2(dh - cy, -cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);


}

void Draw::tinted_Rotated_BITMAP_Region(double dx, double dy, double dw, double dh, double sx, double sy, double sw, double sh, double cx, double cy, float a, Color color, Bitmap * bmp)
{
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(color.r, color.g, color.b, color.a);


	glBindTexture(GL_TEXTURE_2D, bmp->GetTex());
	glBegin(GL_QUADS);

	double dist;
	float a2;
	Point2D p1, p2;

	glTexCoord2d(sx / bmp->GetW(), sy / bmp->GetH());
	dist = sqrt(pow(cx, 2) + pow(cy, 2));
	a2 = atan2(-cy, -cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d((sx + sw) / bmp->GetW(), sy / bmp->GetH());
	dist = sqrt(pow(dw - cx, 2) + pow(cy, 2));
	a2 = atan2(-cy, dw - cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);



	glTexCoord2d((sx + sw) / bmp->GetW(), (sy + sh) / bmp->GetH());
	dist = sqrt(pow(dw - cx, 2) + pow(dh - cy, 2));
	a2 = atan2(dh - cy, dw - cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);


	glTexCoord2d(sx / bmp->GetW(), (sy + sh) / bmp->GetH());
	dist = sqrt(pow(cx, 2) + pow(dh - cy, 2));
	a2 = atan2(dh - cy, -cx);

	glVertex3d(dx + cx + cos(a2 + a) * dist, dy + cy + sin(a2 + a) * dist, 0.1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

// Camera
Camera::Camera()
{

}

void Camera::Perspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	
	double w = double(Display_info::width) / _zoom, h = double(Display_info::height) / _zoom;
	gluOrtho2D(0, w, h, 0);
	
	glRotatef(_c, 0, 0, 1);

	glTranslated(-_pos.X(), -_pos.Y(), -1);


}

void Camera::GUI()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 2D
	double w = double(Display_info::width), h = double(Display_info::height);
	gluOrtho2D(0, w, h, 0);


	glRotatef(0, 0, 0, 1);

	glTranslatef(0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
}

void Camera::FlatPerspective()
{
}

void Camera::Update()
{
	if (_follow != nullptr)
	{
		_pos.Set(_follow->X(), _follow->Y());
	}

	if (_x_limit_l != _x_limit_r)
	{
		if (_pos.X() < _x_limit_l)
			_pos.SetX(_x_limit_l); 
		if (_pos.X() + Display_info::width / _zoom > _x_limit_r)
			_pos.SetX(_x_limit_r - Display_info::width / _zoom);
		
		if (_x_limit_r - _x_limit_l < Display_info::width / _zoom)
			_pos.SetX((_x_limit_r + _x_limit_l) * 0.5  - 0.5 * Display_info::width / _zoom);

	}

	if (_y_limit_u != _y_limit_d)
	{
		if (_pos.Y() < _y_limit_u)
			_pos.SetY(_y_limit_u);
		if (_pos.Y() + Display_info::height / _zoom > _y_limit_d) 
			_pos.SetY(_y_limit_d - Display_info::height / _zoom);
		if (_y_limit_d - _y_limit_u < Display_info::height / _zoom)
			_pos.SetY((_y_limit_d + _y_limit_u) * 0.5 - 0.5 * Display_info::height / _zoom);
	}
	//cout << _pos.X() << " : " << _x_limit_r << endl;
}

void Camera::Follow(Point2D * p)
{
	_follow = p;
}

void Camera::GoTo(Point2D p)
{
	_pos.Set(p.X(), p.Y());
}

void Camera::Rotate(float x, float y, float z)
{
	_a = x; _b = y; _c = z;
}

void Camera::RotateX(float x)
{
	_a = x;
}

void Camera::RotateY(float y)
{
	_b = y;
}

void Camera::RotateZ(float z)
{
	_c = z;
}

void Camera::SetFOV(double fov)
{
	_fov = fov;
}

void Camera::SetZoom(double zoom)
{
	_zoom = zoom;
}

void Camera::SetPos(double x, double y, double z)
{
	_pos.Set(x, y);
}

void Camera::SetLimit(double lx, double rx, double uy, double dy)
{
	_x_limit_l = lx; _x_limit_r = rx;
	_y_limit_d = dy; _y_limit_u = uy;
}

double Camera::X()
{
	return _pos.X();
}

double Camera::Y()
{
	return _pos.Y();
}

void Camera::SetX(double x)
{
	_pos.SetX(x);
}

void Camera::SetY(double y)
{
	_pos.SetY(y);
}
