#pragma once

#include"SDL.h"
#include"SDL_ttf.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<algorithm> 
#include<stdio.h>
#include<string.h>
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
#include<windows.h>
#endif
#include "Polygon.h"
#include "WorldObject.h"

class CRenderer
{
public:
	SDL_Surface* screen;
	SDL_Surface* font;
	SDL_Surface* strings; 
	TTF_Font* tmpfont;
	int width;
	int height;
	int font_init;

	void StartDrawing();
	void StopDrawing();
	void FlipScreen();
	void ClearScreenToColor(int color);

	int InitFont(char* fontfile, int fontsize);
	int DrawText(const char* text, int posx, int posy);
	void PrintNumber(int x, int y, int num);
	void ScoreAndFPS(int x, int y, int fps, int score, int lives);

	void PutPixel(int x, int y, int color);
	void ThickLine(Vector p1, Vector p2, int color);
	void Line(Vector p1, Vector p2, int color);
	void Line(int startx, int starty, int endx, int endy, int color);
	void Polygon(PolygonVertices* _polygon, int color);
	void PolygonFilled(PolygonVertices* _polygon, int color);
	
	//triangle??
	void TriangleFilled(PolygonVertices* _polygon, int color);

	void Circle(Vector center, int radius, int color);
	//out of bounds Polygon/Circle/PolygonFilled/Line ... ?

	CRenderer(int screen_width, int screen_height);
	~CRenderer(void);
};
