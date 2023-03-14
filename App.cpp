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

	restartGame();

	displayWindows(SW_SHOWNA);
	for(auto window : popupWindows)
		UpdateWindow(window->getHandle());
}

void App::restartGame()
{
	Tile::currentLetter = 0;
	Tile::currentRow = 0;

	for (auto window : popupWindows)
	{
		auto item = dictionary.begin();
		std::advance(item, rand() % dictionary.size());
		window->word = item->first;
		SetWindowText(window->handle, window->word.c_str());
	}
}

void App::updateAfterKeyInput(wchar_t pressed)
{
	if (Tile::currentLetter >= Tile::wordSize || Tile::currentRow >= Tile::numberOfTries)return;

	for (auto window : popupWindows)
		window->updateOnKeyPressed(pressed);
	
	InvalidateRect(mainWindow.handle, nullptr, FALSE);
	Tile::currentLetter++;
	typed += pressed;
}

void App::updateBackspace()
{
	if (Tile::currentLetter <= 0)return;
	for (auto window : popupWindows)
	{
		window->tiles[Tile::currentLetter - 1][Tile::currentRow].setLetter(' ');
		InvalidateRect(window->handle, nullptr, FALSE);
	}
	Tile::currentLetter--;
	typed = typed.substr(0, typed.size() - 1);
}

void App::updateEnter()
{
	if (Tile::currentLetter < Tile::wordSize || Tile::currentRow >= Tile::numberOfTries)return;

	for (auto window : popupWindows)
		window->updateOnEnter(dictionary.find(typed) != dictionary.end());

	Tile::currentLetter = 0;
	Tile::currentRow += (dictionary.find(typed) != dictionary.end());
	typed = L"";
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

	std::ifstream dict("Wordle.txt");
	while (!dict.eof())
	{
		std::string tmp;
		dict >> tmp;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper);
		dictionary[std::wstring(tmp.begin(), tmp.end())] = true;
	}
	auto it = dictionary.begin();

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

		while (!toDelete.empty())
		{
			delete toDelete.top();
			toDelete.pop();
		}
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
