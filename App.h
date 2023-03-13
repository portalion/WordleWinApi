#pragma once
#include "KeyboardWindow.h"

class App
{
private:
	HINSTANCE m_instance;

	KeyboardWindow mainWindow;

	void displayWindows(int show_command);

public:
	App(HINSTANCE instance);
	int run(int showCommand);

};

