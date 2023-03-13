#include "Tile.h"

Tile::Tile(int posx, int posy) :
	pos{ .left = posx, .top = posy, .right = posx + size, .bottom = posy + size}, 
	letter{ L"A" }, actual{Color::None}
{
}

void Tile::draw(HDC* hdc)
{
	HGDIOBJ hPen = nullptr;
	HGDIOBJ hOldPen = nullptr;
	HGDIOBJ hBrush = nullptr;
	HGDIOBJ hOldBrush = nullptr;

	hPen = CreatePen(PS_SOLID, 1 + (actual == Color::None), actual == Color::None ? stateToColor(Color::Bad) : stateToColor(actual));
	hBrush = CreateSolidBrush(stateToColor(actual));
	hOldPen = SelectObject(*hdc, hPen);
	hOldBrush = SelectObject(*hdc, hBrush);

	RoundRect(*hdc, pos.left, pos.top, pos.right, pos.bottom, elipseSize, elipseSize);
	DrawText(*hdc, letter, 1, &pos, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(*hdc, hOldPen);
	SelectObject(*hdc, hOldBrush);
	
	DeleteObject(hBrush);
	DeleteObject(hPen);
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
