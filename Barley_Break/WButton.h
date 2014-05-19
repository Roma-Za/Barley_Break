#pragma once
#include <windows.h>

//---------------------------------------------------------------------------
class WButton
{
public:
	WButton();
	virtual ~WButton();

	HWND Create(HINSTANCE hinst, HWND parent, int size, HMENU hMenu, int x, int y);
	BOOL Show(int dCmdShow = SW_SHOWNORMAL);
	operator HWND();
protected:
	HWND hwnd;
	HINSTANCE mhInst;

public:
	HINSTANCE GetInstance();

private:
};

void WINAPI CustomRegister(void);
void WINAPI CustomUnregister(void);
