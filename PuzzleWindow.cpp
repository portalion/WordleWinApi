#include "PuzzleWindow.h"

PuzzleWindow::PuzzleWindow(HINSTANCE instance, HWND parent, Difficulty& amount)
    : amount{amount}, tiles{Tile::wordSize}
{
    registerClass(instance);

    RECT size{ 0, 0, (Tile::size + Tile::margin) * Tile::wordSize + 3 * Tile::margin, 
        (Tile::size + Tile::margin) * Tile::numberOfTries + 3 * Tile::margin};


    AdjustWindowRectEx(&size, WS_OVERLAPPED | WS_CAPTION, false, 0);
    const POINT position =
    {
        .x = (GetSystemMetrics(SM_CXSCREEN) - size.right) / 2,
        .y = (GetSystemMetrics(SM_CYSCREEN) - size.bottom) / 2
    };
    handle = CreateWindowEx(0, nameOfClass.c_str(), L"WORDLE - PUZZLE",
        WS_OVERLAPPED | WS_CAPTION,
        position.x, position.y, size.right - size.left, size.bottom - size.top,
        parent, nullptr, instance, nullptr);

    SetWindowLongPtrW(handle, 0, reinterpret_cast<LONG_PTR>(this));
    SetLayeredWindowAttributes(handle, 0, 255 * 50 / 100, LWA_ALPHA);

    for (int i = 0; i < Tile::wordSize; i++)
        for (int j = 0; j < Tile::numberOfTries; j++)
            tiles[i].push_back(Tile(2 * Tile::margin + i * (Tile::size + Tile::margin),
                2 * Tile::margin + j * (Tile::size + Tile::margin), ' '));
}

bool PuzzleWindow::registerClass(HINSTANCE instance)
{
    WNDCLASSEX wc{};

    if (GetClassInfoExW(instance, nameOfClass.c_str(), &wc) != 0)return true;

    wc =
    {
        .cbSize = sizeof(WNDCLASSEX),
        .lpfnWndProc = PuzzleWindow::staticWindowProc,
        .cbWndExtra = sizeof(PuzzleWindow*),
        .hInstance = instance,
        .hbrBackground = CreateSolidBrush(RGB(255, 255, 255)),
        .lpszClassName = nameOfClass.c_str()
    };

    return RegisterClassEx(&wc) != 0;
}

LRESULT PuzzleWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(handle);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProc(hwnd, message, wparam, lparam);
        if (hit == HTCLIENT) hit = HTCAPTION;
        return hit;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(hwnd, &ps);

        for (auto row : tiles)
            for(auto tile : row)
                tile.draw(hdc);

        EndPaint(hwnd, &ps);
        return 0;
    }
    
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}
