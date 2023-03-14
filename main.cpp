#define NOMINMAX
#include "App.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previnstance, PWSTR cmdline, int nCmdShow)
{
	App* app = App::getInstance(instance);
	int result = app->run(nCmdShow);
	App::releaseInstance();
	return result;
}