#pragma once
#include "Window.h"
#include "Tile.h"
#include <vector>

class PuzzleWindow : public Window<PuzzleWindow>
{
private:
	std::wstring nameOfClass = L"PuzzleWindow";
	Difficulty& amount;

	bool registerClass(HINSTANCE instance);

	std::vector<std::vector<Tile>> tiles;
public:
	PuzzleWindow(HINSTANCE instance, HWND parent, Difficulty& amount);
	LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

