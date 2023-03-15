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

	int time = 0;

	std::wstring word;
	bool inGame = true;
	bool inAnimation = false;
	bool registerClass(HINSTANCE instance);
	void updateOnKeyPressed(wchar_t typed);
	void updateOnEnter(bool good);
	void updateInGame();
public:
	PuzzleWindow(HINSTANCE instance, HWND parent, int id);
	LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	friend App;
};

