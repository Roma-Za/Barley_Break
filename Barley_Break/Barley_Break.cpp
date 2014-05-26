// Barley_Break.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Barley_Break.h"
#include "WButton.h"

#define MAX_LOADSTRING 100
#define SIZE_CLIENT_SQUARE 400
#define BUTTON_SIZE 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����
HWND ButtonEmpty = NULL;
bool isClassic = true;
bool isSelectionOfPictures = false;
bool dblClick = false;
int timeOfGame = 0;
int klick = 0;
int size;
int NumBitmap; 
int xStart,yStart,xFinish,yFinish;
RECT rcWindow = {0, 0, SIZE_CLIENT_SQUARE, SIZE_CLIENT_SQUARE}; //������ ���������� ������� ���� ��� ������� ���� ��� ��� �������� � ������������� �������� ��������
RECT clientImageRect;
HWND hWnd;
HWND OK = NULL;
HWND CAN�EL = NULL;
HWND hwndButtons [16] ={0};
//��������� ����������� ����� �� ��������
static HBITMAP hBitmaps[16] = { LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3) ),
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6) ),
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP7) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9) ),
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP10) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP11) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP12) ),
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP13) ),
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP14) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP15) ), 
	LoadBitmap( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP16) ) };


static HBITMAP CurrentBitmap;
BITMAP bmp;
std::vector<int> vectorNumberBitmap;
POINT ArrPoints[16];


OPENFILENAME ofn;				// ��������� �����. ����� �������
TCHAR szFile[260] = {0};       // ����� ��� ����� �����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void ChangeButton(HWND pressedButton);
void Refresh();
void DistributeBitmaps();
bool ItsWin();
HBITMAP ULLoadImage(LPCTSTR szResource);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;
	//������������ ��������� ������� ��������� �����
	CustomRegister();

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BARLEY_BREAK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BARLEY_BREAK));

	// ���� ��������� ���������:

	while (GetMessage (&msg, NULL, 0, 0))
	{
		//����� ������� ����� ������ ���� �� �������
		if (msg.hwnd == hwndButtons[0]||msg.hwnd == hwndButtons[1]||msg.hwnd == hwndButtons[2]||msg.hwnd == hwndButtons[3]||
			msg.hwnd == hwndButtons[4]||msg.hwnd == hwndButtons[5]||msg.hwnd == hwndButtons[6]||msg.hwnd == hwndButtons[7]||
			msg.hwnd == hwndButtons[8]||msg.hwnd == hwndButtons[9]||msg.hwnd == hwndButtons[10]||msg.hwnd == hwndButtons[11]||
			msg.hwnd == hwndButtons[12]||msg.hwnd == hwndButtons[13]||msg.hwnd == hwndButtons[14]||msg.hwnd == hwndButtons[15]) {
				switch (msg.message) {
				case WM_LBUTTONDOWN:
					ChangeButton(msg.hwnd);
					continue;
				}

		}
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
//  �����������:
//
//    ��� ������� � �� ������������� ���������� ������ � ������, ���� �����, ����� ������ ���
//    ��� ��������� � ��������� Win32, �� �������� ������� RegisterClassEx'
//    ������� ���� ��������� � Windows 95. ����� ���� ������� ����� ��� ����,
//    ����� ���������� �������� "������������" ������ ������ � ���������� �����
//    � ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BARLEY_BREAK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BARLEY_BREAK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

	return RegisterClassEx(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


	hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������
	//�������� ������ ���� ��������������� ������� ������� ���������� �������
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, true);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WM_HSCROLL | WM_VSCROLL,
		CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, hInstance, NULL);
	Refresh();
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdc_bit;
	HDC hMemDC;

	switch (message)
	{

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{

		case 17://������� �� ���� ��
			{
				//����������� ���� ��������� ������� ������ ��� 100�100
				if((clientImageRect.right - clientImageRect.left) > 100 && (clientImageRect.bottom - clientImageRect.top) > 100){
					size = (clientImageRect.right - clientImageRect.left)/4; //������ ����� ����������� ��� ����� ��������� �����
					for (int i = 0; i<16; i++) //���������� ����� ������� ����� ��������� ��� ����������� �� ������
					{
						ArrPoints[i].x = size*(i % 4)+clientImageRect.left;
						ArrPoints[i].y = size*(i / 4)+clientImageRect.top;
					}
					//������ ������ �� � cancel
					ShowWindow(OK, SW_HIDE);	
					ShowWindow(CAN�EL, SW_HIDE);
					//���������� ��������� �����
					for (int i = 0; i < 16; ++i) 
					{      		
						ShowWindow(hwndButtons[i], SW_SHOW);	
					}
					isSelectionOfPictures = false;		
					Refresh(); // ���������� ����� � ������ ����� ����
					//���������� ������ ���������� ������� ���� ��������������� ������� ��������� ����� �����������
					WORD nWidth = clientImageRect.right - clientImageRect.left; // LOWORD(lParam);  ������ ���������� �������
					WORD nHeight = clientImageRect.bottom - clientImageRect.top; //HIWORD(lParam);  ������ ���������� �������
					DWORD lParam = nWidth | (nHeight<<16);
					WPARAM wParam = (WPARAM)SIZE_RESTORED;
					SendMessage(hWnd, WM_SIZE, wParam, lParam);
				}
			}break;
		case 18://������� �� ���� CANCEL
			{
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	
				}
				isSelectionOfPictures = false;			
				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CAN�EL, SW_HIDE);
				MessageBox(hWnd,L"�������� ������ ����������� ���� ������������ ������� � ������������� ������� (Classic)", L"���������", MB_OK);
			} break;
		case IDM_OPEN: 
			// ���������� �� ������ ���������� ���� ������� (Open).
			xStart = yStart = xFinish = yFinish = 0;
			KillTimer(hWnd, 1);
			SetWindowText(hWnd, L"����� �����������");	
			if (GetOpenFileName(&ofn)==TRUE) {

				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);

				CurrentBitmap = ULLoadImage(ofn.lpstrFile); //������ bitmap �� ���������� ������������� �����������

				GetObject(CurrentBitmap, sizeof (BITMAP), &bmp); //�������� ������ �������
				//������ �����
				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_HIDE);	
				}
				//�������� OK � CAN�EL
				ShowWindow(OK, SW_SHOW);	
				ShowWindow(CAN�EL, SW_SHOW);
				//���������� ������ ���������� ������� ���� ��������������� ������� ��������� ����� �����������, 
				//���� ������ ����������� ������ ���������� ������, ������ ��������������� �� ������� �������
				RECT desktopArea;
				//�������� ������ ������ ��� ������ �����
				SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopArea, 0);

				WORD nWidth = bmp.bmWidth; 
				WORD nHeight = bmp.bmHeight;
				//����� ���������� ��������� �������� ������ ���� �� 40�50 ��� ������ ���������� ������� ����� 40�50, � ���� ���� ������.
				//� ������ �������� ����������� ��� ���� ��������� ���� ���� �� x,y.
				// padding ��������� ����� ��� ��������� � ����� ����
				int padding = 100;
				WORD maxSide = nWidth > nHeight ? nWidth : nHeight;
				WORD destSize;

				if (maxSide < desktopArea.right && maxSide < desktopArea.bottom) {
					destSize = maxSide;
				} else {
					destSize = desktopArea.bottom - padding;
				}
				DWORD lParam = destSize | (destSize<<16);
				WPARAM wParam = (WPARAM)SIZE_RESTORED;
				SendMessage(hWnd, WM_SIZE, wParam, lParam);

				isSelectionOfPictures = true;
				isClassic = false;
				InvalidateRect(hWnd,NULL,NULL);
				UpdateWindow(hWnd);

			}
			break;
		case ID_Classic: //���������� ������������ ������� �������� � �������
			{ 	
				isClassic = true;
				isSelectionOfPictures = false;		
				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	//���������� �����
				}

				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CAN�EL, SW_HIDE);
				//������� ������ ���������� ������� ���� ����� ��� ��� �������� ����  
				DWORD lParam = rcWindow.right | ( rcWindow.bottom<<16);
				WPARAM wParam = (WPARAM)SIZE_RESTORED;
				SendMessage(hWnd, WM_SIZE, wParam, lParam);

				Refresh();
				InvalidateRect(hWnd,NULL,NULL);
				UpdateWindow(hWnd);
			} break;

		case IDM_ABOUT: 	
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:
		{
			//��������� ������������ ������� �������� �����
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Image\0*.JPG;*.GIF;*.BMP\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			//�������� 16 -� ������, ������� ��������� ���� �����
			for (int i=0; i < 16; i++) {
				WButton button;
				hwndButtons [i] = button.Create(GetModuleHandle(NULL), hWnd, BUTTON_SIZE, (HMENU)(i+1), BUTTON_SIZE * (i % 4), BUTTON_SIZE * (i / 4));
			}
			RECT rect;
			GetClientRect(hWnd,&rect);
			// �������� ������ OK � CAN�EL, ������� ������������ ��� ������ ����� �����������
			OK = CreateWindow ( L"BUTTON", L"OK", WS_CHILD | BS_PUSHBUTTON,  rect.right - 130, rect.bottom-35, 60, 30,hWnd, (HMENU)17, GetModuleHandle(NULL), NULL);
			CAN�EL = CreateWindow ( L"BUTTON", L"CANCEL", WS_CHILD | BS_PUSHBUTTON, rect.right - 65, rect.bottom-35, 60, 30,hWnd, (HMENU)18, GetModuleHandle(NULL), NULL);

		}break;

	case WM_PAINT:
		{
			HDC ArrComHDC[16];
			hdc = BeginPaint(hWnd, &ps);
			if(!isSelectionOfPictures){ //���� �� ����� ������ �����������
				RECT rect_butt;
				PAINTSTRUCT ArrPS[16] = {0};
				HDC ArrHDC[16];
				GetClientRect(hwndButtons[0],&rect_butt); //�������� ������ �����
				if(isClassic)
				{

					for(int i = 0; i < 16; i++){//������ ���� ������ �� ������ ������
						NumBitmap = vectorNumberBitmap[i]; //����� ������� � ������� ��������
						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]); //�������� �������� ���������� ��� ������ ������
						ArrComHDC[i] = CreateCompatibleDC(ArrHDC[i]); //�������� �������� ������
						SelectObject(ArrComHDC[i], hBitmaps[NumBitmap]); //�������� � �������� ������ ������
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[i], 0, 0, 100,100, SRCCOPY);//�������� ������ �� ��������� ������ � �������� ������
					}		
					for(int i = 0; i < 16; i++){//����������� ���������
						DeleteDC(ArrHDC[i]); 
						DeleteDC(ArrComHDC[i]); 
					}
				}
				else//������� ���� � ������������
				{
					for(int i = 0; i < 16; i++)
					{

						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]); //�������� �������� ���������� ��� ������ ������
					}
					ArrComHDC[0] = CreateCompatibleDC(ArrHDC[0]); //����� ���� �������� ������
					SelectObject(ArrComHDC[0], CurrentBitmap); //�������� �����������
					for(int i = 0; i < 16; i++)
					{		
						NumBitmap = vectorNumberBitmap[i];  //����� ����� ������� 
						//�������� ��������������� ����� ����������� �� ������ ������
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[0], 
							ArrPoints[NumBitmap].x,  ArrPoints[NumBitmap].y, size, size, SRCCOPY);

					}	
					for(int i = 0; i < 16; i++){
						DeleteDC(ArrHDC[i]);
					}
					DeleteDC(ArrComHDC[0]); 
				}
			} else{//����� ������ ����� ����������� ��� ����

				ArrComHDC[0] = CreateCompatibleDC(hdc); //����� ���� �������� ������
				SelectObject(ArrComHDC[0], CurrentBitmap); //�������� ������ ����������� �������������
				StretchBlt(hdc,0, 0,  bmp.bmWidth, bmp.bmHeight, ArrComHDC[0], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //�������� ������ �� �������� �������� ����
				DeleteDC(ArrComHDC[0]); //����������� �������� ������
				SelectObject (hdc, GetStockObject (NULL_BRUSH)); //�������� ���������� �����		
				//���������� ������� �������� ������ ����� �����������
				int MinSizeClientRect = abs(xFinish-xStart) < abs(yFinish-yStart) ? abs(xFinish-xStart) : abs(yFinish-yStart);
				int left, right, top, bottom;
				if (xStart < xFinish) {
					left = xStart;
					right = xStart + MinSizeClientRect;
				} else {
					left = xStart - MinSizeClientRect;
					right = xStart;
				}

				if (yStart < yFinish) {
					top = yStart;
					bottom = yStart + MinSizeClientRect;
				} else {
					top = yStart - MinSizeClientRect;
					bottom = yStart;
				}
				//������ �������
				Rectangle(hdc, left, top, right, bottom);
				//���������� �������������� ������� � ��������� RECT
				clientImageRect.left = left;
				clientImageRect.top = top;
				clientImageRect.right = right;
				clientImageRect.bottom = bottom;

			}
			EndPaint(hWnd, &ps);
		} break;

	case WM_LBUTTONDOWN:
		{
			//���������� ��������� ����� ��������
			dblClick = false;
			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			break;
		}
	case WM_LBUTTONUP:
		if (!dblClick)//���������� ������� ������
		{
			//�������� �����
			xFinish = LOWORD(lParam);
			yFinish = HIWORD(lParam);
			InvalidateRect(hWnd,NULL,NULL);
			UpdateWindow(hWnd);

		}
		break;
	case WM_MOUSEMOVE:
		if(MK_LBUTTON == wParam)//���� ������ ������
		{  //�������� �����
			xFinish = LOWORD(lParam);
			yFinish = HIWORD(lParam);
			InvalidateRect(hWnd,NULL,NULL);
			UpdateWindow(hWnd);
		}
		break;

	case WM_SIZE:
		{
			RECT rcWindow;
			RECT rect;
			//������� ���������� ������� ����
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			//������� ����
			GetWindowRect(hWnd,&rect);
			int wWidth = rect.right - rect.left;
			int wHeight = rect.bottom - rect.top;
			int wMax = wWidth>wHeight?wWidth:wHeight;
			int cMax = width>height?width:height;
			//����������� ������� ������ (�����)
			double deltaButt = SIZE_CLIENT_SQUARE/BUTTON_SIZE;
			double sizeButton = ((double)cMax)/deltaButt;
			rcWindow.left = 0; rcWindow.top = 0; rcWindow.right = cMax; rcWindow.bottom = cMax; 
			AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, true);//�������� ������ ���� ��������������� ������� ������� ���������� �������
			//�������������� ���� � ������ ��������� � ������������
			MoveWindow(hWnd, rect.left, rect.top, rcWindow.right - rcWindow.left , rcWindow.bottom - rcWindow.top, true);
			//�������������� ������ � ������ ��������� � ������������
			for (int i = 0; i < 16; i++)
				MoveWindow(hwndButtons[i], sizeButton*(i%4), sizeButton*(i/4), sizeButton, sizeButton, true);
			//���������������� ������ OK � CAN�EL 
			GetClientRect(hWnd,&rect);
			MoveWindow(OK, rect.right - 130, rect.bottom-35, 60, 30, true);
			MoveWindow(CAN�EL, rect.right - 65, rect.bottom-35, 60, 30, true);

			SendMessage(hWnd, WM_PAINT, NULL, NULL);
		} break; 
	case WM_TIMER:
		{
			timeOfGame++; //����� ����
			TCHAR buffTime[300] = {0};
			swprintf_s(buffTime,300,L"%d ���. %d ���.",timeOfGame/60, timeOfGame%60);
			SetWindowText(hWnd, buffTime);					
		} break;
	case WM_CLOSE:
		for (int i = 0; i < 16; i++)
		{
			DeleteObject(hBitmaps[i]);
		}
		if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		for (int i = 0; i < 16; i++)
		{
			DeleteObject(hBitmaps[i]);
		}
		if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return TRUE;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void ChangeButton(HWND pressedButton){
	//���������� ID ������� ������ (�����) � ������ ������
	int ID_Empty = GetWindowLong (ButtonEmpty, GWL_ID);
	int ID_pressed = GetWindowLong (pressedButton, GWL_ID);

	if((ID_pressed-1==ID_Empty && ID_pressed!=5 && ID_pressed!=9 && ID_pressed!=13)||
		(ID_pressed+1==ID_Empty&& ID_pressed!=4 && ID_pressed!=8 && ID_pressed!=12)||
		ID_pressed-4==ID_Empty||
		ID_pressed+4==ID_Empty)
	{//���� ������ � ������� �������� 

		klick++;//������� ���������� ������

		std::swap ( vectorNumberBitmap[ID_Empty-1] , vectorNumberBitmap[ID_pressed-1] );//������ ������� ����� �������� � ������� ������� ��� ����� �������� 
		DistributeBitmaps();//������� ���������� ������� ������ � �������� �����

		if(ItsWin()){//���� ������
			KillTimer(hWnd, 1);//������������� ������
			//���������� ���������� ������ (�����), � ����� ���� 
			TCHAR BuffMessVictory[300] = {0};
			swprintf_s(BuffMessVictory,300,L"������! �� %d ������ � %d ���. %d ���.", klick, timeOfGame/60, timeOfGame%60);
			MessageBox(hWnd,BuffMessVictory, L"���������", MB_OK);				
		} 

	}

}

void Refresh(){//������ ����� ����
	vectorNumberBitmap.clear();
	for (int i = 0; i < 16; ++i) //��������� ������ ��������
	{ 
		vectorNumberBitmap.push_back(i);
	}

	std::random_shuffle( vectorNumberBitmap.begin(), vectorNumberBitmap.end() );//���������� 

	DistributeBitmaps();
	//�������� �������� � ��������� ������
	klick = 0;
	timeOfGame = 0;
	SetTimer(hWnd,1,1000,NULL );
	SetWindowText(hWnd, L"time");
}

void DistributeBitmaps()
{
	ShowWindow(ButtonEmpty, SW_SHOW);//�������� ���������� ������� ������

	for (int i = 0; i < 16; ++i) 
	{      
		if(vectorNumberBitmap[i]==15)//������� ������ ������ 15 (������ ������)
		{
			ButtonEmpty = hwndButtons[i];//����������� ������ ������ �� ���������� ������� ��� ������
			ShowWindow(hwndButtons[i], SW_HIDE);//�������� ����� ������ ������
		}
	}
}
bool ItsWin(){//���������� ������ ��� ���
	int counter = 0;
	for (int i = 0; i < 16; ++i) //���� ������ ������������� �������� ������ ������ 
	{ 
		if(vectorNumberBitmap[i]!=i) //��� ������ ������������ ������ � ������� ���������� false
		{
			return false;
		}
		else
		{
			counter++;
		}	
	}	
	if(counter==15)//���� ������� ���, �� ���������� true
	{
		return true;
	}
}

HBITMAP ULLoadImage(LPCTSTR szResource)
{
	HRESULT hr;
	IStream* pStream;
	IPicture* m_pPicture;   
	LPVOID pNewMem=NULL;

	//������ ����� �� �����
	hr=SHCreateStreamOnFile(szResource,STGM_SHARE_EXCLUSIVE,&pStream);
	if(hr!=S_OK)
		return NULL;

	//��������� �������� �� ������
	hr = OleLoadPicture(pStream,0,true,IID_IPicture,(void**)&m_pPicture);
	if((hr!=S_OK)||(m_pPicture==NULL))
		return NULL;
	HBITMAP hBitmap=NULL;
	//�������� ����� �������
	m_pPicture->get_Handle((OLE_HANDLE*)(UINT*)&hBitmap);
	//�������� ������ �������
	BITMAP bmp;
	if(GetObject(hBitmap,sizeof(BITMAP),&bmp)==0)
		return NULL;
	//�������� ������
	HDC hDTDC=GetDC(NULL);
	HDC hSrcDC=CreateCompatibleDC(hDTDC);
	hBitmap=(HBITMAP)SelectObject(hSrcDC,hBitmap);
	HDC hDestDC=CreateCompatibleDC(hDTDC);
	HBITMAP hBitmapRet=CreateCompatibleBitmap(hDTDC,bmp.bmWidth,bmp.bmHeight);
	hBitmapRet=(HBITMAP)SelectObject(hDestDC,hBitmapRet);
	BitBlt(hDestDC,0,0,bmp.bmWidth,bmp.bmHeight,hSrcDC,0,0,SRCCOPY);
	SelectObject(hSrcDC,hBitmap);
	hBitmapRet=(HBITMAP)SelectObject(hDestDC,hBitmapRet);
	//����������� �������
	DeleteDC(hSrcDC);
	DeleteDC(hDestDC);    
	ReleaseDC(NULL,hDTDC);
	if(pNewMem!=NULL)
		GlobalFree(pNewMem);
	m_pPicture->Release();
	pStream->Release();
	return hBitmapRet;
}
