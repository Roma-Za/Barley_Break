// Barley_Break.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Barley_Break.h"
#include "WButton.h"

#define MAX_LOADSTRING 100
#define SIZE_CLIENT_SQUARE 400
#define BUTTON_SIZE 100
#define DISPLAY_HEIGHT 750
#define DISPLAY_WIDTH 1300

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
HWND ButtonEmpty = NULL;
bool isClassic = true;
bool isSelectionOfPictures = false;
bool dblClick = false;
int timeOfGame = 0;
int klick = 0;
int size;
int NumBitmap; 
int xStart,yStart,xFinish,yFinish;
RECT rcWindow = {0, 0, SIZE_CLIENT_SQUARE, SIZE_CLIENT_SQUARE};
RECT clientImageRect;
HWND hWnd;
HWND OK = NULL;
HWND CANСEL = NULL;
HWND hwndButtons [16] ={0};
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


OPENFILENAME ofn;       // структура станд. блока диалога
TCHAR szFile[260] = {0};       // буфер для имени файла

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void ChangeButton(HWND pressedButton);
void Refresh();
char* ws2s(const LPWSTR s);
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

	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;
	CustomRegister();

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BARLEY_BREAK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BARLEY_BREAK));

	// Цикл основного сообщения:

	while (GetMessage (&msg, NULL, 0, 0))
	{

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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
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
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, true);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WM_HSCROLL | WM_VSCROLL,
		CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
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

		case 17:
			{

				if(clientImageRect.right!=0){
					size = (clientImageRect.right - clientImageRect.left)/4;
					for (int i = 0; i<16; i++) 
					{
						ArrPoints[i].x = size*(i % 4)+clientImageRect.left;
						ArrPoints[i].y = size*(i / 4)+clientImageRect.top;
					}
					ShowWindow(OK, SW_HIDE);	
					ShowWindow(CANСEL, SW_HIDE);
					for (int i = 0; i < 16; ++i) 
					{      		
						ShowWindow(hwndButtons[i], SW_SHOW);	
					}
					isSelectionOfPictures = false;		
					Refresh();
					WORD nWidth = clientImageRect.right - clientImageRect.left; // LOWORD(lParam);   width of client area 
					WORD nHeight = clientImageRect.bottom - clientImageRect.top; //HIWORD(lParam);  height of client area
					DWORD lParam = nWidth | (nHeight<<16);
					WPARAM wParam = (WPARAM)SIZE_RESTORED;
					SendMessage(hWnd, WM_SIZE, wParam, lParam);
				}
			}break;
		case 18:
			{
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	
				}
				isSelectionOfPictures = false;			
				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CANСEL, SW_HIDE);
			} break;
		case IDM_OPEN: 
			// Показываем на экране диалоговое окно Открыть (Open).
			xStart = yStart = xFinish = yFinish = 0;
			if (GetOpenFileName(&ofn)==TRUE) {

				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);

				CurrentBitmap = ULLoadImage(ofn.lpstrFile);

				GetObject(CurrentBitmap, sizeof (BITMAP), &bmp);

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_HIDE);	
				}
				ShowWindow(OK, SW_SHOW);	
				ShowWindow(CANСEL, SW_SHOW);

				WORD nWidth = bmp.bmWidth; // LOWORD(lParam);   width of client area 
				WORD nHeight = bmp.bmHeight; //HIWORD(lParam);  height of client area

				double scaleW = (double) nWidth / DISPLAY_WIDTH; 
				double scaleH = (double) nHeight / DISPLAY_HEIGHT;
				double maxScale = scaleW > scaleH ? scaleW : scaleH;
				maxScale = maxScale > 1 ? maxScale : 1;

				DWORD lParam = (int)(nWidth/maxScale) | ((int)(nHeight/maxScale)<<16);
				WPARAM wParam = (WPARAM)SIZE_RESTORED;
				SendMessage(hWnd, WM_SIZE, wParam, lParam);

				isSelectionOfPictures = true;
				isClassic = false;
				InvalidateRect(hWnd,NULL,NULL);
				UpdateWindow(hWnd);

			}
			break;
		case ID_Classic:
			{ 	
				isClassic = true;
				isSelectionOfPictures = false;		
				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	
				}

				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CANСEL, SW_HIDE);

				DWORD lParam = rcWindow.right | ( rcWindow.bottom<<16);
				WPARAM wParam = (WPARAM)SIZE_RESTORED;
				SendMessage(hWnd, WM_SIZE, wParam, lParam);

				Refresh();
				InvalidateRect(hWnd,NULL,NULL);
				UpdateWindow(hWnd);
			} break;

		case IDM_ABOUT: 	// Разобрать выбор в меню:
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
			// Инициализация структуры OPENFILENAME
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

			for (int i=0; i < 16; i++) {
				WButton button;
				hwndButtons [i] = button.Create(GetModuleHandle(NULL), hWnd, BUTTON_SIZE, (HMENU)(i+1), BUTTON_SIZE * (i % 4), BUTTON_SIZE * (i / 4));
			}
			RECT rect;
			GetClientRect(hWnd,&rect);

			OK = CreateWindow ( L"BUTTON", L"OK", WS_CHILD | BS_PUSHBUTTON,  rect.right - 130, rect.bottom-35, 60, 30,hWnd, (HMENU)17, GetModuleHandle(NULL), NULL);
			CANСEL = CreateWindow ( L"BUTTON", L"CANCEL", WS_CHILD | BS_PUSHBUTTON, rect.right - 65, rect.bottom-35, 60, 30,hWnd, (HMENU)18, GetModuleHandle(NULL), NULL);
			Refresh();
		}break;

	case WM_PAINT:
		{
			HDC ArrComHDC[16];
			hdc = BeginPaint(hWnd, &ps);
			if(!isSelectionOfPictures){
				RECT rect_butt;
				PAINTSTRUCT ArrPS[16] = {0};
				HDC ArrHDC[16];
				GetClientRect(hwndButtons[0],&rect_butt);
				if(isClassic)
				{
				
					for(int i = 0; i < 16; i++){
						NumBitmap = vectorNumberBitmap[i]; 
						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]);
						ArrComHDC[i] = CreateCompatibleDC(ArrHDC[i]);
						SelectObject(ArrComHDC[i], hBitmaps[NumBitmap]);
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[i], 0, 0, 100,100, SRCCOPY);
					}		
					for(int i = 0; i < 16; i++){
						DeleteDC(ArrHDC[i]); 
						DeleteDC(ArrComHDC[i]); 
					}
				}
				else
				{
					for(int i = 0; i < 16; i++)
					{

						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]);
					}
					ArrComHDC[0] = CreateCompatibleDC(ArrHDC[0]);
					SelectObject(ArrComHDC[0], CurrentBitmap);
					for(int i = 0; i < 16; i++)
					{		
						NumBitmap = vectorNumberBitmap[i]; 
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[0], 
						ArrPoints[NumBitmap].x,  ArrPoints[NumBitmap].y, size, size, SRCCOPY);

					}	
					for(int i = 0; i < 16; i++){
						DeleteDC(ArrHDC[i]);
					}
					DeleteDC(ArrComHDC[0]); 
				}
			} else{

				ArrComHDC[0] = CreateCompatibleDC(hdc);
				SelectObject(ArrComHDC[0], CurrentBitmap);
				StretchBlt(hdc,0, 0,  bmp.bmWidth, bmp.bmHeight, ArrComHDC[0], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
				DeleteDC(ArrComHDC[0]); 
				SelectObject (hdc, GetStockObject (NULL_BRUSH)); 		

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

				Rectangle(hdc, left, top, right, bottom);

				clientImageRect.left = left;
				clientImageRect.top = top;
				clientImageRect.right = right;
				clientImageRect.bottom = bottom;

			}
			EndPaint(hWnd, &ps);
		} break;

	case WM_LBUTTONDOWN:
		{
			dblClick = false;
			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			break;
		}
	case WM_LBUTTONUP:
		if (!dblClick)
		{
			xFinish = LOWORD(lParam);
			yFinish = HIWORD(lParam);
			InvalidateRect(hWnd,NULL,NULL);
			UpdateWindow(hWnd);

		}
		break;
	case WM_MOUSEMOVE:
		if(MK_LBUTTON == wParam)
		{  
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
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			GetWindowRect(hWnd,&rect);
			int wWidth = rect.right - rect.left;
			int wHeight = rect.bottom - rect.top;
			int wMax = wWidth>wHeight?wWidth:wHeight;
			int cMax = width>height?width:height;

			double deltaButt = SIZE_CLIENT_SQUARE/BUTTON_SIZE;
			double sizeButton = ((double)cMax)/deltaButt;
			rcWindow.left = 0; rcWindow.top = 0; rcWindow.right = cMax; rcWindow.bottom = cMax; 
			AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, true);

			MoveWindow(hWnd, rect.left, rect.top, rcWindow.right - rcWindow.left , rcWindow.bottom - rcWindow.top, true);

			for (int i = 0; i < 16; i++)
				MoveWindow(hwndButtons[i], sizeButton*(i%4), sizeButton*(i/4), sizeButton, sizeButton, true);
			GetClientRect(hWnd,&rect);
			MoveWindow(OK, rect.right - 130, rect.bottom-35, 60, 30, true);
			MoveWindow(CANСEL, rect.right - 65, rect.bottom-35, 60, 30, true);

			SendMessage(hWnd, WM_PAINT, NULL, NULL);
		} break; 
	case WM_TIMER:
		{
			timeOfGame++;
			TCHAR buffTime[300] = {0};
			swprintf_s(buffTime,300,L"%d мин. %d сек.",timeOfGame/60, timeOfGame%60);
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

// Обработчик сообщений для окна "О программе".
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
	int ID_Empty = GetWindowLong (ButtonEmpty, GWL_ID);
	int ID_pressed = GetWindowLong (pressedButton, GWL_ID);

	if((ID_pressed-1==ID_Empty && ID_pressed!=5 && ID_pressed!=9 && ID_pressed!=13)||
		(ID_pressed+1==ID_Empty&& ID_pressed!=4 && ID_pressed!=8 && ID_pressed!=12)||
		ID_pressed-4==ID_Empty||
		ID_pressed+4==ID_Empty)
	{

		klick++;

		std::swap ( vectorNumberBitmap[ID_Empty-1] , vectorNumberBitmap[ID_pressed-1] );
		DistributeBitmaps();

		if(ItsWin()){
			KillTimer(hWnd, 1);
			TCHAR BuffMessVictory[300] = {0};
			swprintf_s(BuffMessVictory,300,L"Победа! За %d кликов и %d мин. %d сек.", klick, timeOfGame/60, timeOfGame%60);
			MessageBox(hWnd,BuffMessVictory, L"Сообщение", MB_OK);				
		} 

	}
	else{
		RECT rect_butt;
		ShowWindow(pressedButton, SW_HIDE);
		ShowWindow(pressedButton, SW_SHOW);
		NumBitmap = vectorNumberBitmap[ID_pressed-1]; 
		HDC hdcPress = GetDC(pressedButton);
		GetClientRect(pressedButton,&rect_butt);
		HDC hdcCom = CreateCompatibleDC(hdcPress);
		if(isClassic)
		{
			SelectObject(hdcCom, hBitmaps[NumBitmap]);
			StretchBlt(hdcPress,0, 0, rect_butt.right, rect_butt.bottom, hdcCom, 0, 0, 50,50, SRCCOPY);
		}
		else{

			SelectObject(hdcCom, CurrentBitmap);
			StretchBlt(hdcPress,0, 0, rect_butt.right, rect_butt.bottom, hdcCom, ArrPoints[NumBitmap].x,  ArrPoints[NumBitmap].y, size, size, SRCCOPY);

		}
		DeleteDC(hdcCom); 
		ReleaseDC(pressedButton, hdcPress);

	}
}

char* ws2s(const LPWSTR s)
{
	char* dest = new char[1000];
	WideCharToMultiByte(CP_ACP, 0, s, -1, dest, strlen(dest),0,0); 
	return dest;
}

void Refresh(){
	vectorNumberBitmap.clear();
	for (int i = 0; i < 16; ++i) 
	{ 
		vectorNumberBitmap.push_back(i);
	}
	srand(time(0));
	std::random_shuffle( vectorNumberBitmap.begin(), vectorNumberBitmap.end() );

	DistributeBitmaps();

	klick = 0;
	timeOfGame = 0;
	SetTimer(hWnd,1,1000,NULL );
}

void DistributeBitmaps()
{
	ShowWindow(ButtonEmpty, SW_SHOW);

	for (int i = 0; i < 16; ++i) 
	{      
		if(vectorNumberBitmap[i]==15)
		{
			ButtonEmpty = hwndButtons[i];
			ShowWindow(hwndButtons[i], SW_HIDE);
		}
	}
}
bool ItsWin(){
	int counter = 0;
	for (int i = 0; i < 16; ++i) 
	{ 
		if(vectorNumberBitmap[i]!=i)
		{
			return false;
		}
		else
		{
			counter++;
		}	
	}	
	if(counter==15)
	{
		return true;
	}
}

HBITMAP ULLoadImage(LPCTSTR szResource)
{
	HRESULT hr;
	IStream* pStream;
	IPicture* m_pPicture;
	HRSRC rc=NULL;          
	LPVOID lpImage=NULL;
	HGLOBAL hgl=NULL;
	DWORD dwSize=0;
	LPVOID pNewMem=NULL;

	//создаём поток из фаила
	hr=SHCreateStreamOnFile(szResource,STGM_SHARE_EXCLUSIVE,&pStream);
	if(hr!=S_OK)
		return NULL;

	//загружаем картинку из потока
	hr = OleLoadPicture(pStream,0,true,IID_IPicture,(void**)&m_pPicture);
	if((hr!=S_OK)||(m_pPicture==NULL))
		return NULL;
	HBITMAP hBitmap=NULL;
	//получаем хендл битмапа
	m_pPicture->get_Handle((OLE_HANDLE*)(UINT*)&hBitmap);
	//получаем размер битмапа
	BITMAP bmp;
	if(GetObject(hBitmap,sizeof(BITMAP),&bmp)==0)
		return NULL;
	//копируем битмап
	HDC hDTDC=GetDC(NULL);
	HDC hSrcDC=CreateCompatibleDC(hDTDC);
	hBitmap=(HBITMAP)SelectObject(hSrcDC,hBitmap);
	HDC hDestDC=CreateCompatibleDC(hDTDC);
	HBITMAP hBitmapRet=CreateCompatibleBitmap(hDTDC,bmp.bmWidth,bmp.bmHeight);
	hBitmapRet=(HBITMAP)SelectObject(hDestDC,hBitmapRet);
	BitBlt(hDestDC,0,0,bmp.bmWidth,bmp.bmHeight,hSrcDC,0,0,SRCCOPY);
	SelectObject(hSrcDC,hBitmap);
	hBitmapRet=(HBITMAP)SelectObject(hDestDC,hBitmapRet);
	//освобождаем ресурсы
	DeleteDC(hSrcDC);
	DeleteDC(hDestDC);    
	ReleaseDC(NULL,hDTDC);
	if(pNewMem!=NULL)
		GlobalFree(pNewMem);
	m_pPicture->Release();
	pStream->Release();
	return hBitmapRet;
}
