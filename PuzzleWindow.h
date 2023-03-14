#pragma once
#include "Window.h"
#include "Tile.h"
#include <vector>

class App;

class PuzzleWindow : public Window<PuzzleWindow>
{
private:
	int id;
	std::wstring nameOfClass = L"PuzzleWindow";
	std::vector<std::vector<Tile>> tiles;

	bool registerClass(HINSTANCE instance);
public:
	PuzzleWindow(HINSTANCE instance, HWND parent, int id);
	LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	friend App;
};

