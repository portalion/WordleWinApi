#include "App.h"

void App::displayWindows(int showCommand)
{
	ShowWindow(mainWindow.getHandle(), showCommand);
}

App::App(HINSTANCE instance)
	: m_instance{instance}, mainWindow{instance}
{
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
	return EXIT_SUCCESS;
}
