#include "App.h"
#include <fstream>

void App::displayWindows(int showCommand)
{
	ShowWindow(mainWindow.getHandle(), showCommand);
	ShowWindow(popupWindow1.getHandle(), showCommand);
}

App::App(HINSTANCE instance)
	: m_instance{instance}, mainWindow{instance},
	popupWindow1{instance, mainWindow.getHandle(), difficulty}
{
	std::ifstream ifs("Wordle.ini");
	int diffFromFile = 1;
	std::string tmp;
	ifs >> tmp >> tmp;
	diffFromFile = tmp[tmp.size() - 1] - '0';

	if (diffFromFile != 1 && diffFromFile != 2 && diffFromFile != 4)diffFromFile = 1;

	difficulty = static_cast<Difficulty>(diffFromFile);
}

int App::run(int showCommand)
{
	displayWindows(showCommand);
	difficulty = Difficulty::MEDIUM;
	MSG msg{};
	BOOL result = TRUE;
	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1)
			return EXIT_FAILURE;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	std::ofstream ofs("Wordle.ini");

	ofs << "[WORDLE]\n";
	ofs << "Difficulty=" << static_cast<int>(difficulty) << '\n';

	return EXIT_SUCCESS;
}
