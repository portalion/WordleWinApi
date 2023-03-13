#include "KeyboardWindow.h"

bool KeyboardWindow::registerClass(HINSTANCE instance)
{
    WNDCLASSEX wc{};

    if (GetClassInfoExW(instance, nameOfClass.c_str(), &wc) != 0)return true;

    wc =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .lpfnWndProc = KeyboardWindow::staticWindowProc,
        .cbWndExtra = sizeof(KeyboardWindow*),
        .hInstance = instance,
        .hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APP_ICON)),
        .hbrBackground = CreateSolidBrush(RGB(255, 255, 255)),
        .lpszClassName = nameOfClass.c_str()
    };

    return RegisterClassEx(&wc) != 0;
}

KeyboardWindow::KeyboardWindow(HINSTANCE instance)
    :Window{}
{
    registerClass(instance);

    const POINT size = { .x = 600, .y = 200 };
    const POINT position =
    {
        .x = (GetSystemMetrics(SM_CXSCREEN) - size.x) / 2,
        .y = (GetSystemMetrics(SM_CYSCREEN) + size.y) / 2
    };

	handle = CreateWindowEx(WS_EX_LAYERED, nameOfClass.c_str(), L"WORDLE - KEYBOARD",
		WS_OVERLAPPED | WS_SYSMENU |
		WS_CAPTION | WS_MINIMIZEBOX,
		position.x, position.y, size.x, size.y, nullptr, nullptr, instance, nullptr);

    SetWindowLongPtrW(handle, 0, reinterpret_cast<LONG_PTR>(this));

    SetLayeredWindowAttributes(handle, 0, 255 * 50 / 100, LWA_ALPHA);
}

LRESULT KeyboardWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(handle);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;

	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}
