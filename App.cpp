#include "App.h"
#include <fstream>

App* App::instance = nullptr;

void App::displayWindows(int showCommand)
{
	ShowWindow(mainWindow.getHandle(), showCommand);
	for(auto window : popupWindows)
		ShowWindow(window->getHandle(), SW_SHOWNA);
}

void App::ChangeDifficulty()
{
	for (auto window : popupWindows)
	{
		DestroyWindow(window->getHandle());
		toDelete.push(window);
	}
	popupWindows.clear();

	Tile::numberOfTries = Tile::difficulty == Difficulty::EASY ? 6 : 6 + static_cast<int>(Tile::difficulty);
	for (int i = 0; i < static_cast<int>(Tile::difficulty); i++)
		popupWindows.push_back(new PuzzleWindow(m_instance, mainWindow.getHandle(), i));

	displayWindows(SW_SHOWNA);
	for(auto window : popupWindows)
		UpdateWindow(window->getHandle());
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
#include <iostream>
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

		while (!toDelete.empty())
		{
			delete toDelete.top();
			toDelete.pop();
		}
		if (toDelete.empty())
			std::cout << "a";
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
