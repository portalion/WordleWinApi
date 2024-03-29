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
	static const int maxNumberOfWindows = 4;
	static int numberOfTries;
	static Difficulty difficulty;
	static const int AnimationTime = 100;

	static int currentRow;
	static int currentLetter;
	//static const COLORREF fontColor;

	inline void setLetter(wchar_t letter) { this->letter[0] = letter; }
	inline wchar_t getLetter() { return letter[0]; }
	inline Color getColor() { return actual; }

	void setColor(Color c);
	Tile(int posx, int posy, wchar_t letter);
	void draw(HDC hdc, int time = 0);
	void drawKeyboard(HDC hdc, Color colors[]);
};

