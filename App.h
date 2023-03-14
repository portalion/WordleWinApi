#pragma once
#include "KeyboardWindow.h"
#include "PuzzleWindow.h"

class App
{
private:
	HINSTANCE m_instance;

	KeyboardWindow mainWindow;
	std::vector<PuzzleWindow*> popupWindows;

	void displayWindows(int show_command);
	void setPopups();

public:
	App(HINSTANCE instance);
	int run(int showCommand);

};

