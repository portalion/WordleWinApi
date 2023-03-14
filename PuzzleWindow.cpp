#include "PuzzleWindow.h"
#include "App.h"


PuzzleWindow::PuzzleWindow(HINSTANCE instance, HWND parent, int id)
    : tiles{Tile::wordSize}, id{id}
{
    registerClass(instance);

    RECT size{ 0, 0, (Tile::size + Tile::margin) * Tile::wordSize + 3 * Tile::margin, 
        (Tile::size + Tile::margin) * Tile::numberOfTries + 3 * Tile::margin};


    AdjustWindowRectEx(&size, WS_OVERLAPPED | WS_CAPTION, false, 0);
    POINT position =
    {
        .x = GetSystemMetrics(SM_CXSCREEN) / 2,
        .y = GetSystemMetrics(SM_CYSCREEN) / 2
    };

    if (Tile::difficulty != Difficulty::EASY)
    {
        position.x /= 2;
        position.x *= 2 * (id % 2) + 1;
    }
    if (Tile::difficulty == Difficulty::HARD)
    {
        position.y /= 2;
        position.y *= id >= 2 ? 3 : 1;
    }

    position.x -= size.right / 2;
    position.y -= size.bottom / 2;
    

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

void PuzzleWindow::updateOnEnter(bool good)
{
    if (!inGame)return;

    if (!good)
    {
        for (int i = 0; i < Tile::wordSize; i++)
            tiles[i][Tile::currentRow].setLetter(' ');
        InvalidateRect(handle, nullptr, FALSE);
        return;
    }

    bool won = true;

    for (int i = 0; i < Tile::wordSize; i++)
    {
        wchar_t letter = tiles[i][Tile::currentRow].getLetter();

        if (word[i] != letter) won = false;

        if (word[i] == letter)tiles[i][Tile::currentRow].setColor(Color::Good);
        else if (word.find(letter) != std::string::npos)tiles[i][Tile::currentRow].setColor(Color::Misplaced);
        else tiles[i][Tile::currentRow].setColor(Color::Bad);
    }

    inGame = !won;

    InvalidateRect(handle, nullptr, TRUE);
}

void PuzzleWindow::updateOnKeyPressed(wchar_t pressed)
{
    if (!inGame)return;
    tiles[Tile::currentLetter][Tile::currentRow].setLetter(pressed);
    InvalidateRect(handle, nullptr, TRUE);
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
    case WM_NCHITTEST: { //https://stackoverflow.com/questions/7773771/how-do-i-implement-dragging-a-window-using-its-client-area
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
