#pragma once
#include "KeyboardWindow.h"
#include "PuzzleWindow.h"
#include <stack>

class App
{
private:
	static App* instance;
	HINSTANCE m_instance;

	KeyboardWindow mainWindow;
	std::vector<PuzzleWindow*> popupWindows;
	std::stack<PuzzleWindow*> toDelete;

	App(HINSTANCE instance);
	void displayWindows(int show_command);
	void ChangeDifficulty();

public:
	int run(int showCommand);

	static App* getInstance(HINSTANCE hinstance = nullptr);
	static void releaseInstance(); //Call only in main
	friend LRESULT KeyboardWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};

