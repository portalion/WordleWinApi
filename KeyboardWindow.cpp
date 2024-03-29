#include "KeyboardWindow.h"
#include "App.h"

const std::wstring KeyboardWindow::KeyboardLayout = L"QWERTYUIOPASDFGHJKLZXCVBNM";

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
        .lpszMenuName = MAKEINTRESOURCE(IDM_DIFFICULTYMENU),
        .lpszClassName = nameOfClass.c_str()
    };

    return RegisterClassEx(&wc) != 0;
}

KeyboardWindow::KeyboardWindow(HINSTANCE instance)
    :Window{}, colors{KeyboardLayout.size()}
{
    registerClass(instance);

    RECT size{0, 0, (Tile::size + Tile::margin) * 10 + 4 * Tile::margin,
        (Tile::size + Tile::margin) * 4 + 4 * Tile::margin};

    const POINT position =
    {
        .x = (GetSystemMetrics(SM_CXSCREEN) - size.right) / 2,
        .y = (GetSystemMetrics(SM_CYSCREEN) - size.bottom) / 2 + 5 * (Tile::size + Tile::margin)
    };
    AdjustWindowRectEx(&size, WS_SYSMENU |
        WS_CAPTION | WS_MINIMIZEBOX, true, 0);
	handle = CreateWindowEx(WS_EX_LAYERED, nameOfClass.c_str(), L"WORDLE - KEYBOARD",
		WS_OVERLAPPED | WS_SYSMENU |
		WS_CAPTION | WS_MINIMIZEBOX,
		position.x, position.y, size.right, size.bottom, nullptr, nullptr, instance, nullptr);

    SetWindowLongPtrW(handle, 0, reinterpret_cast<LONG_PTR>(this));
    SetLayeredWindowAttributes(handle, 0, 255 * 50 / 100, LWA_ALPHA);

    for (int i = 0; i < 10; i++)
        tiles.push_back(Tile(2 * Tile::margin + i * (Tile::margin + Tile::size), 
            2 * Tile::margin, KeyboardLayout[i]));
    for (int i = 10; i < 19; i++)
        tiles.push_back(Tile(2 * Tile::margin + (Tile::size + Tile::margin) / 2
            + (i - 10) * (Tile::margin + Tile::size),
            2 * Tile::margin + Tile::size + Tile::margin, KeyboardLayout[i]));
    for (int i = 19; i < 26; i++)
        tiles.push_back(Tile(2 * Tile::margin + (Tile::size + Tile::margin) / 2
            + (i - 18) * (Tile::margin + Tile::size),
            2 * (Tile::margin + Tile::size + Tile::margin), KeyboardLayout[i]));

    for (int i = 0; i < colors.size(); i++)colors[i] = new Color[Tile::maxNumberOfWindows];

    resetColors();
}

void KeyboardWindow::resetColors()
{
    for (auto color : colors)
        for (int i = 0; i < Tile::maxNumberOfWindows; i++)
            color[i] = Color::None;

    for (auto tile : tiles)
        tile.setColor(Color::None);
}

LRESULT KeyboardWindow::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(handle);
		return 0;
	case WM_DESTROY:
        for (int i = 0; i < colors.size(); i++)delete[] colors[i];
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wparam);

        switch (wmId)
        {
        case ID_DIFFICULTY_EASY:
            Tile::difficulty = Difficulty::EASY;
            App::getInstance()->ChangeDifficulty();
            //CheckMenuItem(GetMenu(handle), ID_DIFFICULTY_EASY, MF_CHECKED);
            break;
        case ID_DIFFICULTY_MEDIUM:
            Tile::difficulty = Difficulty::MEDIUM;
            App::getInstance()->ChangeDifficulty();
            //CheckMenuItem(GetMenu(handle), ID_DIFFICULTY_MEDIUM, MF_CHECKED);
            break;
        case ID_DIFFICULTY_HARD:
            Tile::difficulty = Difficulty::HARD;
            App::getInstance()->ChangeDifficulty();
            //CheckMenuItem(GetMenu(handle), ID_DIFFICULTY_HARD, MF_CHECKED);
            break;
        }
    }
    break;
    case WM_KEYDOWN:
    {
        if (wparam == VK_BACK)
        {
            App::getInstance()->updateBackspace();
            break;
        }
        if (wparam == VK_RETURN)
        {
            App::getInstance()->updateEnter();
            break;
        }

        wchar_t pressed = static_cast<wchar_t>(wparam);
        if (!isalpha(pressed))
            break;
        App::getInstance()->updateAfterKeyInput(pressed);
    }
        
        break;
    case WM_PAINT: //https://www.robertelder.ca/doublebuffering anti flickerking code
    {
        RECT Client_Rect;
        GetClientRect(handle, &Client_Rect);
        int win_width = Client_Rect.right - Client_Rect.left;
        int win_height = Client_Rect.bottom + Client_Rect.left;
        PAINTSTRUCT ps;
        HDC Memhdc;
        HDC hdc;
        HBITMAP Membitmap;
        hdc = BeginPaint(handle, &ps);
        Memhdc = CreateCompatibleDC(hdc);
        Membitmap = CreateCompatibleBitmap(hdc, win_width, win_height);
        SelectObject(Memhdc, Membitmap);
        //drawing code goes in here
        Rectangle(Memhdc, -10, -10, win_width, win_height);
        for (int i = 0; i < tiles.size(); i++)
            tiles[i].drawKeyboard(Memhdc, colors[i]);

        BitBlt(hdc, 0, 0, win_width, win_height, Memhdc, 0, 0, SRCCOPY);
        DeleteObject(Membitmap);
        DeleteDC(Memhdc);
        DeleteDC(hdc);
        EndPaint(handle, &ps);

    }
        return 0L;
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}
