#pragma once
#include "KeyboardWindow.h"
#include "PuzzleWindow.h"

class App
{
private:
	HINSTANCE m_instance;

	KeyboardWindow mainWindow;
	PuzzleWindow popupWindow1;

	Difficulty difficulty;
	void displayWindows(int show_command);

public:
	App(HINSTANCE instance);
	int run(int showCommand);

};

