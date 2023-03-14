#pragma once
#include "KeyboardWindow.h"
#include "PuzzleWindow.h"
#include <stack>
#include <map>
#include <random>

class App
{
private:
	static App* instance;
	HINSTANCE m_instance;
	std::wstring typed = L"";

	KeyboardWindow mainWindow;
	std::vector<PuzzleWindow*> popupWindows;
	std::stack<PuzzleWindow*> toDelete;

	App(HINSTANCE instance);
	void displayWindows(int show_command);
	void ChangeDifficulty();

	void restartGame();

	void updateAfterKeyInput(wchar_t pressed);
	void updateBackspace();
	void updateEnter();

public:
	int run(int showCommand);

	std::map<std::wstring, bool> dictionary;

	static App* getInstance(HINSTANCE hinstance = nullptr);
	static void releaseInstance(); //Call only in main
	friend LRESULT KeyboardWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};

