#define NOMINMAX
#include "App.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previnstance, PWSTR cmdline, int nCmdShow)
{
	App app(instance);
	return app.run(nCmdShow);
}