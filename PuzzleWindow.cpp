#include "PuzzleWindow.h"
#include "App.h"
#include <wingdi.h>


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

    time = 0;
    SetTimer(handle, id, 10, nullptr);

    if (inAnimation)
    {
        for (int i = 0; i < Tile::wordSize; i++)
        {
            wchar_t letter = tiles[i][animatedRow].getLetter();
            if (word[i] == letter)tiles[i][animatedRow].setColor(Color::Good);
            else if (word.find(letter) != std::string::npos)
                tiles[i][animatedRow].setColor(Color::Misplaced);
            else tiles[i][animatedRow].setColor(Color::Bad);
        }
        animatedRow++;
    }

    inAnimation = true;
    updateInGame();
}

void PuzzleWindow::updateOnKeyPressed(wchar_t pressed)
{
    if (!inGame)return;
    tiles[Tile::currentLetter][Tile::currentRow].setLetter(pressed);
    InvalidateRect(handle, nullptr, FALSE);
}

void PuzzleWindow::updateInGame()
{
    bool won = true;
    for (int i = 0; i < Tile::wordSize; i++)
        if (word[i] != tiles[i][Tile::currentRow].getLetter()) won = false;

    inGame = !won;
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
    case WM_TIMER:
    {
        time += 10;

        if (time >= Tile::AnimationTime * Tile::wordSize)
        {
            KillTimer(handle, id);
            inAnimation = false;
            animatedRow++;
        }
        else
        {
            int animationIndex = time / Tile::AnimationTime;
            wchar_t letter = tiles[animationIndex][animatedRow].getLetter();

            if (word[animationIndex] == letter)tiles[animationIndex][animatedRow].setColor(Color::Good);
            else if (word.find(letter) != std::string::npos)
                tiles[animationIndex][animatedRow].setColor(Color::Misplaced);
            else tiles[animationIndex][animatedRow].setColor(Color::Bad);
        }

        InvalidateRect(handle, nullptr, FALSE);
    }   
        break;
    case WM_DESTROY:
        KillTimer(handle, id);
        break;
    case WM_PAINT:
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

        Rectangle(Memhdc, -10, -10, win_width + 10, win_height + 10);

        for (int i = 0; i < tiles.size(); i++)
            for (int j = 0; j < tiles[i].size(); j++)
                tiles[i][j].draw(Memhdc, j == animatedRow ?
                    (i == (time / Tile::AnimationTime) ? time - Tile::AnimationTime * i : 0) : 0);

        BitBlt(hdc, 0, 0, win_width, win_height, Memhdc, 0, 0, SRCCOPY);

        if ((!inGame  || Tile::currentRow == Tile::numberOfTries) && !inAnimation)
        {
            HGDIOBJ hBrush = CreateSolidBrush(RGB(255 * inGame, 255 * !inGame, 0));
            HGDIOBJ hOldBrush = nullptr;
            HGDIOBJ hPen = CreatePen(PS_SOLID, 0, RGB(255 * inGame, 255 * !inGame, 0));
            HGDIOBJ hOldPen = nullptr;

            hOldBrush = SelectObject(Memhdc, hBrush);
            hOldPen = SelectObject(Memhdc, hPen);

            RECT size;
            GetClientRect(handle, &size);
            Rectangle(Memhdc, size.left, size.top, size.right, size.bottom);
            if (inGame)
            {
                HFONT font = CreateFont(
                    -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 50),
                    0,
                    0,
                    0,
                    FW_BOLD,
                    FALSE,
                    FALSE,
                    0,
                    EASTEUROPE_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_SWISS,
                    L" Verdana ");
                HFONT oldFont = static_cast<HFONT>(SelectObject(Memhdc, font));
                DrawText(Memhdc, word.c_str(), 5, &size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(hdc, oldFont);

                DeleteObject(font);
            }

            SelectObject(Memhdc, hOldPen);
            SelectObject(Memhdc, hOldBrush);

            DeleteObject(hBrush);
            DeleteObject(hPen);

            BLENDFUNCTION fun =
            {
                .BlendOp = AC_SRC_OVER,
                .BlendFlags = 0,
                .SourceConstantAlpha = 150,
            };

            AlphaBlend(hdc, 0, 0, win_width, win_height, Memhdc, 0, 0, win_width, win_height, fun);
        }

        DeleteObject(Membitmap);
        DeleteDC(Memhdc);
        DeleteDC(hdc);
        EndPaint(handle, &ps);

        
        return 0;
    }
    
	}
	return DefWindowProcW(hwnd, message, wparam, lparam);
}
