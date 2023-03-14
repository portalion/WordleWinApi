#define NOMINMAX
#include "App.h"
#include <time.h>

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previnstance, PWSTR cmdline, int nCmdShow)
{
	srand(time(NULL));
	App* app = App::getInstance(instance);
	int result = app->run(nCmdShow);
	App::releaseInstance();
	return result;
}