#include <windows.h>
#include <time.h>
#include "resource1.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define MAXSTYLE 6
#define MAXCOLOR 255
#define MAXHORIZONTAL 800
#define MAXVERTICAL 600

// 전역으로 메모리 DC가 이곳에 bit맵을 그려준다. 
HBITMAP gBitMap;


int createWNDClass(OUT WNDCLASSEX& wcex, IN HINSTANCE hInstance);
int initHWND(OUT HWND& hWnd, IN HINSTANCE hInstance);
int initAppTitle(IN HWND hWnd, IN HINSTANCE hInstance);
int initBackground(IN HDC hdc, IN HWND hWnd);

void makingFigure(HDC hdc, int downPosX, int downPosY);
void makingFollowRect(HDC hdc, int x, int y);

LRESULT CALLBACK WndProc(HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpszCmdParam,
					  int nCmdShow)
{
	srand((unsigned int)time(NULL));

	WNDCLASSEX wcex;
	createWNDClass(wcex, hInstance);
	if (!RegisterClassEx(&wcex))
		return 0;
	
	HWND hWnd;
	initHWND(hWnd, hInstance);
	if (!hWnd)
		return 0;

	if (!initAppTitle(hWnd, hInstance))
		return 0;

	ShowWindow(hWnd, nCmdShow);

	MSG	msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd
						 , UINT message
						 , WPARAM wParam
						 , LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT	ps;

	switch (message)
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_COMMAND:
	{
		//	//lparam과 wparam은 메세지에 따라서 읽는 방법이 다르다
		//switch (LOWORD(wParam)) //id 구분
		//{
		//case ID_FILE_EXIT:
		//	DestroyWindow(hWnd);
		//break;
		//}
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		HDC memoryDC = CreateCompatibleDC(hdc);
		HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
		HBITMAP memoryOldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

		HDC imgDC = CreateCompatibleDC(hdc);
		HBITMAP imgBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_BITMAP1));
		HBITMAP imgOldBitmap = (HBITMAP)SelectObject(imgDC, imgBitmap);


		BitBlt(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL, imgDC, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

		SelectObject(memoryDC, memoryOldBitmap);
		DeleteObject(memoryBitmap);
		DeleteObject(memoryDC);

		SelectObject(imgDC, imgBitmap);
		DeleteObject(imgBitmap);
		DeleteObject(imgDC);

		
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_LBUTTONDOWN:
	{	
		int downPosX = LOWORD(lParam);
		int downPosY = HIWORD(lParam);
		HDC hdc = GetDC(hWnd);
		makingFigure(hdc, downPosX, downPosY);
		ReleaseDC(hWnd, hdc);

		InvalidateRect(hWnd, NULL, false);

	}
	break;
	case WM_RBUTTONDOWN:
	{

	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		//OutputDebugString(L"LBUTTONDC\n");
	}
		break;
	case WM_MOUSEMOVE:
		{
			/*int mouseX = LOWORD(lParam);
			int mouseY = HIWORD(lParam);
			
			HDC hdc = GetDC(hWnd);
			HDC memoryDC = CreateCompatibleDC(hdc);
			HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
			HBITMAP oldBitmap = (HBITMAP) SelectObject(memoryDC, memoryBitmap);
			TCHAR xBuffer[200];
			TCHAR yBuffer[200];

			wsprintf(xBuffer, L"X : %d", mouseX);
			wsprintf(yBuffer, L"Y : %d", mouseY);

			TextOut(memoryDC, 700, 20, xBuffer, wcslen(xBuffer));
			TextOut(memoryDC, 700, 40, yBuffer, wcslen(yBuffer));

			makingFollowRect(memoryDC, mouseX, mouseY);

			BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

			SelectObject(memoryDC, oldBitmap);
			DeleteObject(memoryBitmap);
			DeleteDC(memoryDC);
			ReleaseDC(hWnd, hdc);*/
		}
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



void makingFigure(HDC hdc, int downPosX, int downPosY)
{
	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitMap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);

	int maxRectSize = 400;
	int rectSize = rand() % maxRectSize / 2;

	int maxEllipseSize = 300;
	int ellipseSize = rand() % maxEllipseSize;

	int red = rand() % MAXCOLOR;
	int green = rand() % MAXCOLOR;
	int blue = rand() % MAXCOLOR;

	int style = rand() % MAXSTYLE;

	HBRUSH defaultBrush, makingBrush;
	makingBrush = CreateHatchBrush(style, RGB(red, green, blue));
	defaultBrush = (HBRUSH)SelectObject(memoryDC, makingBrush);
	HBITMAP defaultBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitMap);

	if (rand() % 2 == 0)
	{
		Rectangle(memoryDC, downPosX - rectSize, downPosY - rectSize,
			downPosX + rectSize, downPosY + rectSize);
	}
	else
	{
		Ellipse(memoryDC, downPosX - ellipseSize, downPosY - ellipseSize,
			downPosX + ellipseSize, downPosY + ellipseSize);
	}

	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	SelectObject(memoryDC, defaultBitmap);
	SelectObject(memoryDC, defaultBrush);
	DeleteObject(makingBrush);
	DeleteObject(memoryBitMap);
	DeleteDC(memoryDC);

}

void makingFollowRect(HDC hdc, int x, int y)
{
	HBRUSH defaultBrush, makingBrush;
	makingBrush = CreateSolidBrush(RGB(200, 0, 0));
	defaultBrush = (HBRUSH)SelectObject(hdc, makingBrush);
	
	int rectSize = 50;
	Rectangle(hdc, x, y, x + rectSize, y + rectSize);

	SelectObject(hdc, defaultBrush);
	DeleteObject(makingBrush);
}

int createWNDClass(OUT WNDCLASSEX& wcex, IN HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;

	//내꺼 아이콘 커서 쓰려면 먼저 hIntance여야 하고
	// MAKEINTRSOURCE여야 한다. 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR2));
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//메뉴 추가후 여기서 세팅
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	wcex.lpszClassName = szWindowClass;

	return 1;
}

int initHWND(OUT HWND& hWnd, IN HINSTANCE hInstance)
{
	 hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, szWindowClass
		, szTitle
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, MAXHORIZONTAL
		, MAXVERTICAL
		, NULL
		, NULL
		, hInstance
		, NULL);

	 return 1;
}

int initAppTitle(IN HWND hWnd, IN HINSTANCE hInstance)
{
	//리소스 사용하는 법
	const int titleSTRSize = 256;
	WCHAR appTitle[titleSTRSize] = { 0, };

	if (!LoadString(hInstance, IDS_TITLE, appTitle, titleSTRSize))
		return 0;

	if (!SetWindowText(hWnd, appTitle))
		return 0;

	return 1;
}

int initBackground(IN HDC hdc, IN HWND hWnd)
{
	HDC imgDC = CreateCompatibleDC(hdc);
	HBITMAP imgBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		MAKEINTRESOURCE(IDR_BIT1));
	HBITMAP imgOldBitmap = (HBITMAP)SelectObject(imgDC, imgBitmap);

	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	HBITMAP memoryOldBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);
	
	BitBlt(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL, imgDC, 0, 0, SRCCOPY);

	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	SelectObject(memoryDC, memoryOldBitmap);
	DeleteObject(memoryBitmap);
	DeleteObject(memoryDC);

	SelectObject(imgDC, imgBitmap);
	DeleteObject(imgBitmap);
	DeleteObject(imgDC);

	return 1;
}
