#include "Tile.h"

int Tile::numberOfTries = 6;
Difficulty Tile::difficulty = Difficulty::EASY;
int Tile::currentLetter = 0;
int Tile::currentRow = 0;

Tile::Tile(int posx, int posy, wchar_t letter) :
	pos{ .left = posx, .top = posy, .right = posx + size, .bottom = posy + size}
{
	this->letter[0] = letter;
	setColor(Color::None);
}

void Tile::draw(HDC hdc)
{
	HGDIOBJ hPen = nullptr;
	HGDIOBJ hOldPen = nullptr;
	HGDIOBJ hBrush = nullptr;
	HGDIOBJ hOldBrush = nullptr;

	hPen = CreatePen(PS_SOLID, 1 + (actual == Color::None), borderColor);
	hBrush = CreateSolidBrush(backgroundColor);
	hOldPen = SelectObject(hdc, hPen);
	hOldBrush = SelectObject(hdc, hBrush);

	RoundRect(hdc, pos.left, pos.top, pos.right, pos.bottom, elipseSize, elipseSize);

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, letter, 1, &pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void Tile::drawKeyboard(HDC hdc, Color colors[])
{
	int rectangles = static_cast<int>(difficulty);
	if (rectangles == 1)
	{
		setColor(colors[0]);
		return draw(hdc);
	}

	bool empty = true;
	for (int i = 0; i < rectangles; i++)
		if (colors[i] != Color::None)empty = false;
	if (empty) return draw(hdc);

	HGDIOBJ hPen = nullptr;
	HGDIOBJ hOldPen = nullptr;
	HGDIOBJ hBrush = nullptr;
	HGDIOBJ hOldBrush = nullptr;

	int Xsize = pos.right - pos.left;
	int Ysize = pos.bottom - pos.top;

	for (int i = 0; i < rectangles; i++)
	{
		hPen = CreatePen(PS_SOLID, 1, stateToColor(colors[i]));
		hBrush = CreateSolidBrush(stateToColor(colors[i]));
		hOldPen = SelectObject(hdc, hPen);
		hOldBrush = SelectObject(hdc, hBrush);

		RoundRect(hdc, 
			pos.left + (Xsize / 2) * (i % 2),
			pos.top + (rectangles == 2 ? 0 : (Ysize / 2) * (i / 2)),
			pos.right - (Xsize / 2) * ((i + 1) % 2),
			pos.bottom - (rectangles == 2 ? 0 : (Ysize / 2) * ((3 - i) / 2)),
			elipseSize, elipseSize);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);

		DeleteObject(hBrush);
		DeleteObject(hPen);
	}
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, letter, 1, &pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

COLORREF stateToColor(Color color)
{
	switch (color)
	{
		case Color::None:
			return RGB(251, 252, 255);
		case Color::Bad:
			return RGB(164, 174, 196);
		case Color::Misplaced:
			return RGB(243, 194, 55);
		case Color::Good:
			return RGB(121, 184, 81);
	}
}

void Tile::setColor(Color c)
{
	actual = c;
	borderColor = actual == Color::None ? stateToColor(Color::Bad) : stateToColor(actual);
	backgroundColor = stateToColor(actual);
}