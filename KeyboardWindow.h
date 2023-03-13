#pragma once
#include "Window.h"

class KeyboardWindow : public Window<KeyboardWindow>
{
private:
	std::wstring nameOfClass = L"KeyboardWindow";
	
	bool registerClass(HINSTANCE instance);

public:
	KeyboardWindow(HINSTANCE instance);
	LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

