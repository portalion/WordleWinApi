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
	InvalidateRect(mainWindow.handle, nullptr, FALSE);

	CheckMenuItem(GetMenu(mainWindow.handle), ID_DIFFICULTY_EASY, MF_UNCHECKED);
	CheckMenuItem(GetMenu(mainWindow.handle), ID_DIFFICULTY_MEDIUM, MF_UNCHECKED);
	CheckMenuItem(GetMenu(mainWindow.handle), ID_DIFFICULTY_HARD, MF_UNCHECKED);

	UINT currentId = Tile::difficulty == Difficulty::EASY ? ID_DIFFICULTY_EASY : 
					 Tile::difficulty == Difficulty::MEDIUM ? ID_DIFFICULTY_MEDIUM :
																ID_DIFFICULTY_HARD;
	CheckMenuItem(GetMenu(mainWindow.handle), currentId, MF_CHECKED);

}

void App::restartGame()
{
	Tile::currentLetter = 0;
	Tile::currentRow = 0;

	mainWindow.resetColors();

	for (auto window : popupWindows)
	{
		auto item = dictionary.begin();
		std::advance(item, rand() % dictionary.size());
		window->word = item->first;
		//SetWindowText(window->handle, window->word.c_str());
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

	bool goodWord = dictionary.find(typed) != dictionary.end();

	for (auto window : popupWindows)
		window->updateOnEnter(goodWord);

	if (goodWord)
	{
		for (int wnd = 0; wnd < popupWindows.size(); wnd++)
		{
			for (int i = 0; i < Tile::wordSize; i++)
			{
				int index = KeyboardWindow::KeyboardLayout.find(
					popupWindows[wnd]->tiles[i][Tile::currentRow].getLetter());
				wchar_t letter = popupWindows[wnd]->tiles[i][Tile::currentRow].getLetter();

				Color c = Color::None;

				if (popupWindows[wnd]->word[i] == letter)c = Color::Good;
				else if (popupWindows[wnd]->word.find(letter) != std::string::npos)
					c = Color::Misplaced;
				else c = Color::Bad;
				

				if (index == std::string::npos)
				{
					index = KeyboardWindow::KeyboardLayout.find(typed[i]);
					c = Color::Bad;
				}

				mainWindow.colors[index][wnd] =
					mainWindow.colors[index][wnd] > c ? mainWindow.colors[index][wnd] : c;
			}
		}
		InvalidateRect(mainWindow.handle, nullptr, FALSE);
	}

	Tile::currentLetter = 0;
	Tile::currentRow += goodWord;
	typed = L"";
}

App::App(HINSTANCE instance)
	: m_instance{instance}, mainWindow{instance}
{
	wchar_t diffFromFile[2];
	GetPrivateProfileString(
		L"WORDLE",
		L"DIFFICULTY",
		L"1",
		diffFromFile,
		2,
		L".\\Wordle.ini"
	);

	Tile::difficulty = static_cast<Difficulty>(diffFromFile[0] - '0');

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

	WritePrivateProfileString(L"WORDLE", L"DIFFICULTY", std::to_wstring(static_cast<int>(Tile::difficulty)).c_str(), L".\\Wordle.ini");

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
