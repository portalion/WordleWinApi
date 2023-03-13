#pragma once
#include <windows.h>
#include <string>

enum class Color
{
	None = 0,
	Bad = 1,
	Misplaced = 2,
	Good = 3
};

COLORREF stateToColor(Color color);

class Tile
{
private:
	LPCWCHAR letter;
	RECT pos;

	COLORREF backgroundColor;
	COLORREF borderColor;
	Color actual;

public:
	static const int size = 55;
	static const int elipseSize = 6;
	//static const COLORREF fontColor;

	Tile(int posx, int posy);
	void draw(HDC* hdc);
};

