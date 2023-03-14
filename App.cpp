#include "App.h"
#include <fstream>

App* App::instance = nullptr;

void App::displayWindows(int showCommand)
{
	ShowWindow(mainWindow.getHandle(), showCommand);
	for(auto window : popupWindows)
		ShowWindow(window->getHandle(), showCommand);
}

void App::ChangeDifficulty()
{
	for (auto window : popupWindows)delete window;
	popupWindows.clear();
	switch (Tile::difficulty)
	{
	case Difficulty::EASY:
		Tile::numberOfTries = 6;
		popupWindows.push_back(new PuzzleWindow(m_instance, mainWindow.getHandle(), 0));
		break;
	case Difficulty::MEDIUM:
		Tile::numberOfTries = 8;
		for(int i = 0; i < 2; i++)
			popupWindows.push_back(new PuzzleWindow(m_instance, mainWindow.getHandle(), i));
		break;
	case Difficulty::HARD:
		Tile::numberOfTries = 10;
		for (int i = 0; i < 4; i++)
			popupWindows.push_back(new PuzzleWindow(m_instance, mainWindow.getHandle(), i));
		break;
	}
}

App::App(HINSTANCE instance)
	: m_instance{instance}, mainWindow{instance}
{
	std::ifstream ifs("Wordle.ini");
	int diffFromFile = 1;
	std::string tmp;
	ifs >> tmp >> tmp;
	diffFromFile = tmp[tmp.size() - 1] - '0';

	if (diffFromFile != 1 && diffFromFile != 2 && diffFromFile != 4)diffFromFile = 1;

	Tile::difficulty = static_cast<Difficulty>(diffFromFile);

	ChangeDifficulty();
}

int App::run(int showCommand)
{
	displayWindows(showCommand);
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
	ofs << "Difficulty=" << static_cast<int>(Tile::difficulty) << '\n';

	return EXIT_SUCCESS;
}

App* App::getInstance(HINSTANCE hinstance)
{
	if (instance != nullptr) return instance;
	
	instance = new App(hinstance);
	return instance;
}

void App::releaseInstance()
{
	delete instance;
	instance = nullptr;
}
