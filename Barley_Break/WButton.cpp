#include "stdafx.h"
#include "WButton.h"

#define CUSTOM_WC              _T("WButton")

//---------------------------------------------------------------------------
WButton::WButton() : hwnd(0)
{
}
//---------------------------------------------------------------------------
WButton::~WButton()
{
}
//---------------------------------------------------------------------------
WButton::operator HWND()
{
	return hwnd;
}
//---------------------------------------------------------------------------
HWND WButton::Create(HINSTANCE hinst, HWND parent, int size, HMENU hMenu, int x, int y)
{
	//Создаем кастомный контрол на основании баттона
	hwnd = CreateWindow(L"WButton", L"", WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT,
		x, y, size, size, parent, hMenu, hinst, NULL);

	return hwnd;
}
//---------------------------------------------------------------------------
BOOL WButton::Show(int dCmdShow)
{
	BOOL CanShow = ::ShowWindow(hwnd, dCmdShow);

	if( CanShow )
		return TRUE;
	return FALSE;
}
//---------------------------------------------------------------------------
HINSTANCE WButton::GetInstance()
{
	return mhInst;
}
//---------------------------------------------------------------------------
static LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Здесь писать обработку сообщений для этого контрола
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void WINAPI
	CustomRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CustomWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//Регистрируем контрол с кастомным названием
	wc.lpszClassName = CUSTOM_WC;
	RegisterClass(&wc);
}

void WINAPI
	CustomUnregister(void)
{
	UnregisterClass(CUSTOM_WC, NULL);
}
