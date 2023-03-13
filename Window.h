#pragma once
#include <windows.h>
#include <string>
#include "resource.h"

//All windows should derive from this, it manages message function to be non-static so we can register class with it
template <class DERIVED_TYPE> 
class Window
{
protected:
	virtual inline std::wstring className() { return nameOfClass; } //Unique classname for registering function
	virtual LRESULT windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND handle = nullptr; //Window handle for this window
	std::wstring nameOfClass;
public:
    static LRESULT CALLBACK staticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //Making non-static function
    {
		DERIVED_TYPE* pwnd = reinterpret_cast<DERIVED_TYPE*>(GetWindowLongPtrW(hwnd, 0)); //Get window struct with calling object

		if (pwnd != NULL) return pwnd->windowProc(hwnd, uMsg, wParam, lParam);
		else return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

	inline HWND getHandle() { return handle; }
};