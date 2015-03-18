#include <windows.h>
#include <time.h>
#include "resource1.h"
#include <math.h>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define MAXSTYLE 6
#define MAXCOLOR 255
#define MAXHORIZONTAL 800
#define MAXVERTICAL 600

struct Coordinate
{
	float posX = 0;
	float posY = 0;
};


float mousePosX = 0;
float mousePosY = 0;

int createWNDClass(OUT WNDCLASSEX& wcex, IN HINSTANCE hInstance);
int initHWND(OUT HWND& hWnd, IN HINSTANCE hInstance);
int initAppTitle(IN HWND hWnd, IN HINSTANCE hInstance);
int initBackground(IN HDC hdc, IN HWND hWnd);
int createLinearAndPoint(IN HDC hdc);
int collisionBoxToBox(IN HDC hdc);
bool boxToBoxCollisonCheck(Coordinate pos, int startX, int startY, int endX, int endY);
bool circleToCircleCollisionCheck(int FirstRadius, Coordinate firstCenterPos,
	int SecondRadius, Coordinate secondCenterPos);
int collisionCircleToCircle(IN HDC hdc);
int collisionBoxToCircle(IN HDC hdc);
int collisionBoxToCircleNew(IN HDC hdc);
bool choiceBoxCheckOrCircleCheck(Coordinate boxStartPos, Coordinate boxEndPos,
	Coordinate circleCenterPos);


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
	
	HDC hdc = GetDC(hWnd);

	// 게임용 메세지 처리 루프 
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//createLinearAndPoint(hdc);
			//collisionBoxToBox(hdc);
			//collisionCircleToCircle(hdc);
			//collisionBoxToCircle(hdc);
			collisionBoxToCircleNew(hdc);
		}
	}
	
	ReleaseDC(hWnd, hdc);


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
	/*	hdc = BeginPaint(hWnd, &ps);
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
		DeleteObject(imgDC);*/

		
		//EndPaint(hWnd, &ps);
	}
		break;
	case WM_LBUTTONDOWN:
	{	
		int downPosX = LOWORD(lParam);
		int downPosY = HIWORD(lParam);
		HDC hdc = GetDC(hWnd);
		//makingFigure(hdc, downPosX, downPosY);
		ReleaseDC(hWnd, hdc);

		//InvalidateRect(hWnd, NULL, false);

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
						 mousePosX = LOWORD(lParam);
						 mousePosY = HIWORD(lParam);
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

int createLinearAndPoint(IN HDC hdc)
{
	//a먼저 선을 그리면

	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	SelectObject(memoryDC, memoryBitmap);

	Rectangle(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL);

	TCHAR xBuffer[200] = { 0, };
	TCHAR yBuffer[200] = { 0, };
	TCHAR dBuffer[200] = { 0, };

	wsprintf(xBuffer, L"X : %d", mousePosX);
	wsprintf(yBuffer, L"Y : %d", mousePosY);

	TextOut(memoryDC, 700, 20, xBuffer, wcslen(xBuffer));
	TextOut(memoryDC, 700, 40, yBuffer, wcslen(yBuffer));

	int linearStartPosX = 300;
	int linearStartPosY = 0;
	int linearEndPosX = 600;
	int linearEndPosY = 600;

	MoveToEx(memoryDC, linearStartPosX, linearStartPosY, NULL);
	LineTo(memoryDC, linearEndPosX, linearEndPosY);

	int a = 2;
	int b = -1;
	int c = -600;

	float denominator = (a * a) + (-b * -b);
	int distance = (a * mousePosX + b * mousePosY + c) / sqrt(denominator);

	wsprintf(dBuffer, L"D : %d", distance);
	TextOut(memoryDC, 700, 60, dBuffer, wcslen(dBuffer));

	int meetX = ((mousePosX) + (2 * mousePosY) + 2 * (-c)) / 5;
	int meetY = 2 * meetX + c;

	MoveToEx(memoryDC, mousePosX, mousePosY, NULL);
	LineTo(memoryDC, meetX, meetY);
	
	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);


	return 1;
}

int collisionBoxToBox(IN HDC hdc)
{
	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	SelectObject(memoryDC, memoryBitmap);

	Rectangle(memoryDC, 0, 0,MAXHORIZONTAL, MAXVERTICAL);

	//create fixedBox
	HBRUSH makingBrush = CreateSolidBrush(RGB(250, 0, 0));
	HBRUSH defaultBrush = (HBRUSH)SelectObject(memoryDC, makingBrush);
	Rectangle(memoryDC, 250, 250, 500, 500);
	
	const int rectPoitSize = 4;
	Coordinate rectCoordi[rectPoitSize];
	rectCoordi[0].posX = mousePosX;
	rectCoordi[0].posY = mousePosY;
	rectCoordi[1].posX = mousePosX;
	rectCoordi[1].posY = mousePosY + 50;
	rectCoordi[2].posX = mousePosX + 50;
	rectCoordi[2].posY = mousePosY;
	rectCoordi[3].posX = mousePosX + 50;
	rectCoordi[3].posY = mousePosY + 50;

	//create mouse FllowBox
	makingBrush = CreateSolidBrush(RGB(0, 250, 0));
	SelectObject(memoryDC, makingBrush);
	
	Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + 50, mousePosY + 50);

	//collision check
	bool collisionCheck = false;
	for (int i = 0; i < rectPoitSize; ++i)
	{
		if (boxToBoxCollisonCheck(rectCoordi[i], 250, 250, 500, 500))
		{
			collisionCheck = true;
		}
	}

	if (collisionCheck)
	{
		makingBrush = CreateSolidBrush(RGB(0, 0, 250));
		SelectObject(memoryDC, makingBrush);
		Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + 50, mousePosY + 50);
	}
	
	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(makingBrush);
	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);

	return 1;
}

int collisionCircleToCircle(IN HDC hdc)
{

	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	SelectObject(memoryDC, memoryBitmap);
	
	Rectangle(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL);

	//create fixedCircle
	HBRUSH makingBrush = CreateSolidBrush(RGB(250, 0, 0));
	HBRUSH defaultBrush = (HBRUSH)SelectObject(memoryDC, makingBrush);

	float fixedCircleCenterX = 300;
	float fixedCircleCenterY = 300;
	Ellipse(memoryDC, fixedCircleCenterX - 100, fixedCircleCenterY - 100,
		fixedCircleCenterX + 100, fixedCircleCenterY + 100);
	int fixedCircleRadius = 100;
	Coordinate fixedCirclePos;
	fixedCirclePos.posX = fixedCircleCenterX;
	fixedCirclePos.posY = fixedCircleCenterY;


	//crate mouseCircle
	makingBrush = CreateSolidBrush(RGB(0, 250, 0));
	SelectObject(memoryDC, makingBrush);
	Ellipse(memoryDC, mousePosX - 50, mousePosY - 50, mousePosX + 50, mousePosY + 50);
	// 마우스 포즈가 원점
	int mouseCircleRadius = 50;

	int distance = sqrt((pow(fixedCircleCenterX - mousePosX, 2) + pow(fixedCircleCenterY - mousePosY, 2)));

	bool checkCollision = false;

	Coordinate mousePos;
	mousePos.posX = mousePosX;
	mousePos.posY = mousePosY;
	if (circleToCircleCollisionCheck(fixedCircleRadius,fixedCirclePos, mouseCircleRadius, mousePos ))
	{
		checkCollision = true;
	}

	if (checkCollision)
	{
		makingBrush = CreateSolidBrush(RGB(0,0 , 250));
		SelectObject(memoryDC, makingBrush);
		Ellipse(memoryDC, mousePosX - 50, mousePosY - 50, mousePosX + 50, mousePosY + 50);
	}

	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(makingBrush);
	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);

	return 1;
}

int collisionBoxToCircle(IN HDC hdc)
{
	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	SelectObject(memoryDC, memoryBitmap);

	Rectangle(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL);

	//create fixedCircle
	HBRUSH makingBrush = CreateSolidBrush(RGB(250, 0, 0));
	HBRUSH defaultBrush = (HBRUSH)SelectObject(memoryDC, makingBrush);

	float fixedCircleCenterX = 300;
	float fixedCircleCenterY = 300;

	Ellipse(memoryDC, fixedCircleCenterX - 100, fixedCircleCenterY - 100,
		fixedCircleCenterX + 100, fixedCircleCenterY + 100);
	int fixedCircleRadius = 100;


	//create mouse FllowBox
	makingBrush = CreateSolidBrush(RGB(0, 250, 0));
	SelectObject(memoryDC, makingBrush);

	int rectSize = 50;
	Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + rectSize, mousePosY + rectSize);

	const int coordiSize = 12;
	Coordinate boxCoordi[coordiSize];
	boxCoordi[0].posX = mousePosX;
	boxCoordi[0].posY = mousePosY;
	boxCoordi[1].posX = mousePosX + rectSize / 3;
	boxCoordi[1].posY = mousePosY;
	boxCoordi[2].posX = mousePosX + (rectSize * 2) / 3;
	boxCoordi[2].posY = mousePosY;
	boxCoordi[3].posX = mousePosX + rectSize;
	boxCoordi[3].posY = mousePosY;
	boxCoordi[4].posX = mousePosX + rectSize;
	boxCoordi[4].posY = mousePosY + rectSize /3;
	boxCoordi[5].posX = mousePosX + rectSize;
	boxCoordi[5].posY = mousePosY + (rectSize * 2) / 3;
	boxCoordi[6].posX = mousePosX + rectSize;
	boxCoordi[6].posY = mousePosY + rectSize;
	boxCoordi[7].posX = mousePosX + (rectSize * 2) / 3;
	boxCoordi[7].posY = mousePosY + rectSize;
	boxCoordi[8].posX = mousePosX + rectSize / 3;
	boxCoordi[8].posY = mousePosY + rectSize;
	boxCoordi[9].posX = mousePosX;
	boxCoordi[9].posY = mousePosY + rectSize;
	boxCoordi[10].posX = mousePosX;
	boxCoordi[10].posY = mousePosY + (rectSize*2) /3;
	boxCoordi[11].posX = mousePosX;
	boxCoordi[11].posY = mousePosY + rectSize/3;

	bool collisonCheck = false;
	for (int i = 0; i < coordiSize; ++i)
	{
		int distance = sqrt(pow(boxCoordi[i].posX - fixedCircleCenterX, 2) +
			pow(boxCoordi[i].posY - fixedCircleCenterY, 2));
		if (distance <= fixedCircleRadius)
		{
			collisonCheck = true;
			break;
		}
	}

	if (collisonCheck)
	{
		makingBrush = CreateSolidBrush(RGB(0, 0, 250));
		SelectObject(memoryDC, makingBrush);

		Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + rectSize, mousePosY + rectSize);
	}

	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(makingBrush);
	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);

	return 1;
}

int collisionBoxToCircleNew(IN HDC hdc)
{

	HDC memoryDC = CreateCompatibleDC(hdc);
	HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
	SelectObject(memoryDC, memoryBitmap);

	Rectangle(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL);

	//create fixedCircle
	HBRUSH makingBrush = CreateSolidBrush(RGB(250, 0, 0));
	HBRUSH defaultBrush = (HBRUSH)SelectObject(memoryDC, makingBrush);

	float fixedCircleCenterX = 300;
	float fixedCircleCenterY = 300;

	Ellipse(memoryDC, fixedCircleCenterX - 100, fixedCircleCenterY - 100,
		fixedCircleCenterX + 100, fixedCircleCenterY + 100);
	int fixedCircleRadius = 100;


	//create mouse FllowBox
	makingBrush = CreateSolidBrush(RGB(0, 250, 0));
	SelectObject(memoryDC, makingBrush);

	int rectSize = 50;
	Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + rectSize, mousePosY + rectSize);

	Coordinate rectCoordinate[4];
	rectCoordinate[0].posX = mousePosX;
	rectCoordinate[0].posY = mousePosY;
	rectCoordinate[1].posX = mousePosX + rectSize;
	rectCoordinate[1].posY = mousePosY;
	rectCoordinate[2].posX = mousePosX;
	rectCoordinate[2].posY = mousePosY + rectSize;
	rectCoordinate[3].posX = mousePosX + rectSize;
	rectCoordinate[3].posY = mousePosY + rectSize;

	Coordinate boxStartPos;
	boxStartPos.posX = mousePosX;
	boxStartPos.posY = mousePosY;

	Coordinate boxEndPos;
	boxEndPos.posX = mousePosX + rectSize;
	boxEndPos.posY = mousePosY + rectSize;

	Coordinate circleCenterPos;
	circleCenterPos.posX = fixedCircleCenterX;
	circleCenterPos.posY = fixedCircleCenterY;

	bool isBoxCheck = choiceBoxCheckOrCircleCheck(boxStartPos, boxEndPos, circleCenterPos);
	
	bool isCollision = false;
	if (isBoxCheck)
	{
		for (int i = 0; i < 4; ++i)
		{
			isCollision = boxToBoxCollisonCheck(rectCoordinate[i], fixedCircleCenterX - fixedCircleRadius,
				fixedCircleCenterY - fixedCircleRadius, fixedCircleCenterX + fixedCircleRadius, fixedCircleCenterY - fixedCircleRadius);
			if (isCollision)
				break;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			isCollision = circleToCircleCollisionCheck(fixedCircleRadius, circleCenterPos, 0, rectCoordinate[i]);
			if (isCollision)
				break;
		}
	}

	if (isCollision)
	{
		makingBrush = CreateSolidBrush(RGB(0, 0, 250));
		SelectObject(memoryDC, makingBrush);
		Rectangle(memoryDC, mousePosX, mousePosY, mousePosX + rectSize, mousePosY + rectSize);
	}

	BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

	DeleteObject(makingBrush);
	DeleteObject(memoryBitmap);
	DeleteDC(memoryDC);

	return 1;
}

bool boxToBoxCollisonCheck(Coordinate eachPos, int startX, int startY, int endX, int endY)
{
	if (eachPos.posX < startX)
		return false;

	if (eachPos.posX > endX)
		return false;

	if (eachPos.posY < startY)
		return false;

	if (eachPos.posY > endY)
		return false;

	return true;

}

bool circleToCircleCollisionCheck(int firstRadius, Coordinate firstCenterPos, 
								  int secondRadius, Coordinate secondCenterPos)
{
	int distance = sqrt(pow(firstCenterPos.posX - secondCenterPos.posX, 2) +
		pow(firstCenterPos.posY - secondCenterPos.posY, 2));

	if (distance > firstRadius + secondRadius)
		return false;

	return true;
}

bool choiceBoxCheckOrCircleCheck(Coordinate boxStartPos, Coordinate boxEndPos, 
								 Coordinate circleCenterPos)
{
	if (circleCenterPos.posX < boxStartPos.posX)
		return false;
	if (circleCenterPos.posX > boxEndPos.posX)
		return false;
	if (circleCenterPos.posY < boxStartPos.posY)
		return false;
	if (circleCenterPos.posY > boxEndPos.posY)
		return false;

	return true;
}
