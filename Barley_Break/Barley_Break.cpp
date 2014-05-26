// Barley_Break.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Barley_Break.h"
#include "WButton.h"

#define MAX_LOADSTRING 100
#define SIZE_CLIENT_SQUARE 400
#define BUTTON_SIZE 100

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
RECT rcWindow = {0, 0, SIZE_CLIENT_SQUARE, SIZE_CLIENT_SQUARE}; //размер клиентской области окна при запуске игры или при переходе к классическому варианту пятнашек
RECT clientImageRect;
HWND hWnd;
HWND OK = NULL;
HWND CANСEL = NULL;
HWND hwndButtons [16] ={0};
//загрузить изображения фишек из ресурсов
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


OPENFILENAME ofn;				// структура станд. блока диалога
TCHAR szFile[260] = {0};       // буфер для имени файла

// Отправить объявления функций, включенных в этот модуль кода:
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
	//Регистрируем кастомный контрол игральной фишкм
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
		//ловим нажатие левой кнопки мыши на батонах
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
	//получаем размер окна соответствующий нужному размеру клиентской области
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

		case 17://Реакция на клик ОК
			{
				//срабатывает если выбранная область больше чем 100х100
				if((clientImageRect.right - clientImageRect.left) > 100 && (clientImageRect.bottom - clientImageRect.top) > 100){
					size = (clientImageRect.right - clientImageRect.left)/4; //размер части изображения для одной игральной фишки
					for (int i = 0; i<16; i++) //координаты левых верхних углов квадратов для отображения на фишках
					{
						ArrPoints[i].x = size*(i % 4)+clientImageRect.left;
						ArrPoints[i].y = size*(i / 4)+clientImageRect.top;
					}
					//скрыть кнопки ок и cancel
					ShowWindow(OK, SW_HIDE);	
					ShowWindow(CANСEL, SW_HIDE);
					//отобразить игральные фишки
					for (int i = 0; i < 16; ++i) 
					{      		
						ShowWindow(hwndButtons[i], SW_SHOW);	
					}
					isSelectionOfPictures = false;		
					Refresh(); // перемешать фишки и начать новую игру
					//установить размер клиентской области окна соответствующий размеру выбранной части изображения
					WORD nWidth = clientImageRect.right - clientImageRect.left; // LOWORD(lParam);  ширина клиентской области
					WORD nHeight = clientImageRect.bottom - clientImageRect.top; //HIWORD(lParam);  высота клиентской области
					DWORD lParam = nWidth | (nHeight<<16);
					WPARAM wParam = (WPARAM)SIZE_RESTORED;
					SendMessage(hWnd, WM_SIZE, wParam, lParam);
				}
			}break;
		case 18://Реакция на клик CANCEL
			{
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	
				}
				isSelectionOfPictures = false;			
				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CANСEL, SW_HIDE);
				MessageBox(hWnd,L"Откройте другое изображение либо классический вариант с нумерованными фишками (Classic)", L"Сообщение", MB_OK);
			} break;
		case IDM_OPEN: 
			// Показываем на экране диалоговое окно Открыть (Open).
			xStart = yStart = xFinish = yFinish = 0;
			KillTimer(hWnd, 1);
			SetWindowText(hWnd, L"Выбор изображения");	
			if (GetOpenFileName(&ofn)==TRUE) {

				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);

				CurrentBitmap = ULLoadImage(ofn.lpstrFile); //создаём bitmap из выбранного пользователем изображения

				GetObject(CurrentBitmap, sizeof (BITMAP), &bmp); //получаем размер битмапа
				//скрыть фишки
				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_HIDE);	
				}
				//показать OK и CANСEL
				ShowWindow(OK, SW_SHOW);	
				ShowWindow(CANСEL, SW_SHOW);
				//установить размер клиентской области окна соответствующий размеру выбранной части изображения, 
				//если размер изображения больше разрешения экрана, размер устанавливается по меньшей стороне
				RECT desktopArea;
				//Получить размер экрана без панели задач
				SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopArea, 0);

				WORD nWidth = bmp.bmWidth; 
				WORD nHeight = bmp.bmHeight;
				//Когда посылается сообщение изменить размер окна на 40х50 это значит клиентская область будет 40х50, а само окно больше.
				//В случае открытия изображения нам надо уменьшить само окно до x,y.
				// padding добавляем запас для заголовка и рамки окна
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
		case ID_Classic: //отображаем классический вариант пятнашек с цифрами
			{ 	
				isClassic = true;
				isSelectionOfPictures = false;		
				if(CurrentBitmap!=NULL) DeleteObject(CurrentBitmap);
				xStart = yStart = xFinish = yFinish = 0;

				for (int i = 0; i < 16; ++i) 
				{      		
					ShowWindow(hwndButtons[i], SW_SHOW);	//отображаем фишки
				}

				ShowWindow(OK, SW_HIDE);	
				ShowWindow(CANСEL, SW_HIDE);
				//передаём размер клиентской области окну такой как при создании окна  
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
			//структура стандартного диалога открытия файла
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
			//создание 16 -и кнопок, которые выполняют роль фишек
			for (int i=0; i < 16; i++) {
				WButton button;
				hwndButtons [i] = button.Create(GetModuleHandle(NULL), hWnd, BUTTON_SIZE, (HMENU)(i+1), BUTTON_SIZE * (i % 4), BUTTON_SIZE * (i / 4));
			}
			RECT rect;
			GetClientRect(hWnd,&rect);
			// создание кнопок OK и CANСEL, которые используются при выборе части изображения
			OK = CreateWindow ( L"BUTTON", L"OK", WS_CHILD | BS_PUSHBUTTON,  rect.right - 130, rect.bottom-35, 60, 30,hWnd, (HMENU)17, GetModuleHandle(NULL), NULL);
			CANСEL = CreateWindow ( L"BUTTON", L"CANCEL", WS_CHILD | BS_PUSHBUTTON, rect.right - 65, rect.bottom-35, 60, 30,hWnd, (HMENU)18, GetModuleHandle(NULL), NULL);

		}break;

	case WM_PAINT:
		{
			HDC ArrComHDC[16];
			hdc = BeginPaint(hWnd, &ps);
			if(!isSelectionOfPictures){ //если не режим выбора изображения
				RECT rect_butt;
				PAINTSTRUCT ArrPS[16] = {0};
				HDC ArrHDC[16];
				GetClientRect(hwndButtons[0],&rect_butt); //получаем размер фишки
				if(isClassic)
				{

					for(int i = 0; i < 16; i++){//рисуем свой битмап на каждую кнопку
						NumBitmap = vectorNumberBitmap[i]; //номер битмапа в массиве битмапов
						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]); //получаем контекст устройства для каждой кнопки
						ArrComHDC[i] = CreateCompatibleDC(ArrHDC[i]); //получаем контекст памяти
						SelectObject(ArrComHDC[i], hBitmaps[NumBitmap]); //выбираем в контекст памяти битмап
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[i], 0, 0, 100,100, SRCCOPY);//копируем битмап из контекста памяти в контекст кнопки
					}		
					for(int i = 0; i < 16; i++){//освобождаем контексты
						DeleteDC(ArrHDC[i]); 
						DeleteDC(ArrComHDC[i]); 
					}
				}
				else//вариант игры с изображением
				{
					for(int i = 0; i < 16; i++)
					{

						ArrHDC[i] = BeginPaint(hwndButtons[i],&ArrPS[i]); //получаем контекст устройства для каждой кнопки
					}
					ArrComHDC[0] = CreateCompatibleDC(ArrHDC[0]); //содаём один контекст памяти
					SelectObject(ArrComHDC[0], CurrentBitmap); //выбераем изображение
					for(int i = 0; i < 16; i++)
					{		
						NumBitmap = vectorNumberBitmap[i];  //номер части битмапа 
						//копируем соответствующую часть изображения на каждую кнопку
						StretchBlt(ArrHDC[i],0, 0, rect_butt.right, rect_butt.bottom, ArrComHDC[0], 
							ArrPoints[NumBitmap].x,  ArrPoints[NumBitmap].y, size, size, SRCCOPY);

					}	
					for(int i = 0; i < 16; i++){
						DeleteDC(ArrHDC[i]);
					}
					DeleteDC(ArrComHDC[0]); 
				}
			} else{//режим выбора части изображения для игры

				ArrComHDC[0] = CreateCompatibleDC(hdc); //содаём один контекст памяти
				SelectObject(ArrComHDC[0], CurrentBitmap); //выбираем битмап загруженный пользователем
				StretchBlt(hdc,0, 0,  bmp.bmWidth, bmp.bmHeight, ArrComHDC[0], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); //копируем битмап на контекст главного окна
				DeleteDC(ArrComHDC[0]); //освобождаем контекст памяти
				SelectObject (hdc, GetStockObject (NULL_BRUSH)); //выбираем прозрачную кисть		
				//определяем размеры квадрата выбора части изображения
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
				//рисуем квадрат
				Rectangle(hdc, left, top, right, bottom);
				//записываем результирующий квадрат в структуру RECT
				clientImageRect.left = left;
				clientImageRect.top = top;
				clientImageRect.right = right;
				clientImageRect.bottom = bottom;

			}
			EndPaint(hWnd, &ps);
		} break;

	case WM_LBUTTONDOWN:
		{
			//запоминаем стартовую точку квадрата
			dblClick = false;
			xStart = LOWORD(lParam);
			yStart = HIWORD(lParam);
			break;
		}
	case WM_LBUTTONUP:
		if (!dblClick)//игнорируем двойной щелчёк
		{
			//конечная точка
			xFinish = LOWORD(lParam);
			yFinish = HIWORD(lParam);
			InvalidateRect(hWnd,NULL,NULL);
			UpdateWindow(hWnd);

		}
		break;
	case WM_MOUSEMOVE:
		if(MK_LBUTTON == wParam)//если кнопка зажата
		{  //конечная точка
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
			//размеры клиентской области окна
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			//размеры окна
			GetWindowRect(hWnd,&rect);
			int wWidth = rect.right - rect.left;
			int wHeight = rect.bottom - rect.top;
			int wMax = wWidth>wHeight?wWidth:wHeight;
			int cMax = width>height?width:height;
			//определение размера кнопок (фишки)
			double deltaButt = SIZE_CLIENT_SQUARE/BUTTON_SIZE;
			double sizeButton = ((double)cMax)/deltaButt;
			rcWindow.left = 0; rcWindow.top = 0; rcWindow.right = cMax; rcWindow.bottom = cMax; 
			AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, true);//получаем размер окна соответствующий нужному размеру клиентской области
			//перерисовываем окно с новыми размерами и координатами
			MoveWindow(hWnd, rect.left, rect.top, rcWindow.right - rcWindow.left , rcWindow.bottom - rcWindow.top, true);
			//перерисовываем кнопки с новыми размерами и координатами
			for (int i = 0; i < 16; i++)
				MoveWindow(hwndButtons[i], sizeButton*(i%4), sizeButton*(i/4), sizeButton, sizeButton, true);
			//позиционирование кнопок OK и CANСEL 
			GetClientRect(hWnd,&rect);
			MoveWindow(OK, rect.right - 130, rect.bottom-35, 60, 30, true);
			MoveWindow(CANСEL, rect.right - 65, rect.bottom-35, 60, 30, true);

			SendMessage(hWnd, WM_PAINT, NULL, NULL);
		} break; 
	case WM_TIMER:
		{
			timeOfGame++; //время игры
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
	//определяем ID нажатой кнопки (фишки) и пустой кнопки
	int ID_Empty = GetWindowLong (ButtonEmpty, GWL_ID);
	int ID_pressed = GetWindowLong (pressedButton, GWL_ID);

	if((ID_pressed-1==ID_Empty && ID_pressed!=5 && ID_pressed!=9 && ID_pressed!=13)||
		(ID_pressed+1==ID_Empty&& ID_pressed!=4 && ID_pressed!=8 && ID_pressed!=12)||
		ID_pressed-4==ID_Empty||
		ID_pressed+4==ID_Empty)
	{//если пустая и нажатая соседние 

		klick++;//подсчёт количества кликов

		std::swap ( vectorNumberBitmap[ID_Empty-1] , vectorNumberBitmap[ID_pressed-1] );//меняем местами номер нажатого и пустого битмапа или части картинки 
		DistributeBitmaps();//функция отображает скрытую кнопку и скрывает новую

		if(ItsWin()){//если победа
			KillTimer(hWnd, 1);//останавливаем таймер
			//показываем количество кликов (ходов), и время игры 
			TCHAR BuffMessVictory[300] = {0};
			swprintf_s(BuffMessVictory,300,L"Победа! За %d кликов и %d мин. %d сек.", klick, timeOfGame/60, timeOfGame%60);
			MessageBox(hWnd,BuffMessVictory, L"Сообщение", MB_OK);				
		} 

	}

}

void Refresh(){//начало новой игры
	vectorNumberBitmap.clear();
	for (int i = 0; i < 16; ++i) //заполняем вектор номерами
	{ 
		vectorNumberBitmap.push_back(i);
	}

	std::random_shuffle( vectorNumberBitmap.begin(), vectorNumberBitmap.end() );//перемешать 

	DistributeBitmaps();
	//обнуляем счётчики и запускаем таймер
	klick = 0;
	timeOfGame = 0;
	SetTimer(hWnd,1,1000,NULL );
	SetWindowText(hWnd, L"time");
}

void DistributeBitmaps()
{
	ShowWindow(ButtonEmpty, SW_SHOW);//показать предыдущую скрытую кнопку

	for (int i = 0; i < 16; ++i) 
	{      
		if(vectorNumberBitmap[i]==15)//находим индекс номера 15 (всегда пустой)
		{
			ButtonEmpty = hwndButtons[i];//присваиваем статус кнопке по найденному индексу как пустая
			ShowWindow(hwndButtons[i], SW_HIDE);//скрываем новую пустую кнопку
		}
	}
}
bool ItsWin(){//определить победа или нет
	int counter = 0;
	for (int i = 0; i < 16; ++i) //если номера соответствуют индексам значит победа 
	{ 
		if(vectorNumberBitmap[i]!=i) //при первом несовпадении номера и индекса возвращаем false
		{
			return false;
		}
		else
		{
			counter++;
		}	
	}	
	if(counter==15)//если совпали все, то возвращаем true
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
