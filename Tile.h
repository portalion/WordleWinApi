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

enum class Difficulty
{
	EASY = 1,
	MEDIUM = 2,
	HARD = 4
};

COLORREF stateToColor(Color color);

class Tile
{
private:
	wchar_t letter[1];
	RECT pos;

	COLORREF backgroundColor;
	COLORREF borderColor;
	Color actual;

public:
	static const int size = 55;
	static const int elipseSize = 6;
	static const int margin = 6;
	static const int wordSize = 5;
	static int numberOfTries;
	//static const COLORREF fontColor;

	Tile(int posx, int posy, wchar_t letter);
	void draw(HDC hdc);
};

