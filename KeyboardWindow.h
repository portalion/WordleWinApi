#pragma once
#include "Window.h"
#include "Tile.h"
#include <vector>

class App;

class KeyboardWindow : public Window<KeyboardWindow>
{
private:
	static const std::wstring KeyboardLayout;
	std::wstring nameOfClass = L"KeyboardWindow";
	
	bool registerClass(HINSTANCE instance);
	std::vector<Tile> tiles;
public:
	KeyboardWindow(HINSTANCE instance);
	LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	friend App;
};

