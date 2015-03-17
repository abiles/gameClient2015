#include <windows.h>
#include <time.h>
#include "resource.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define MAXSTYLE 6
#define MAXCOLOR 255
#define MAXHORIZONTAL 800
#define MAXVERTICAL 600

void makingFigure(HDC hdc);
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
	 
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	
	//���� ������ Ŀ�� ������ ���� hIntance���� �ϰ�
	// MAKEINTRSOURCE���� �Ѵ�. 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	//�޴� �߰��� ���⼭ ����
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
		return 0;

	HWND	hWnd = CreateWindowEx(WS_EX_APPWINDOW
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

	if (!hWnd)
		return 0;

	//���ҽ� ����ϴ� ��
	const int titleSTRSize = 256;
	WCHAR appTitle[titleSTRSize];
	LoadString(hInstance, IDS_TITLE, appTitle, titleSTRSize);
	SetWindowText(hWnd, appTitle);

	ShowWindow(hWnd, nCmdShow);

	MSG	msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// �޽��� ó�� �Լ�
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
		break;
	case WM_COMMAND:
		{
			//lparam�� wparam�� �޼����� ���� �д� ����� �ٸ���

			switch (LOWORD(wParam)) //id ����
			{
			case ID_FILE_EXIT:
				DestroyWindow(hWnd);
			}
		}
	case WM_PAINT:
	{
	   
		{

		hdc = BeginPaint(hWnd, &ps);
		//// ���� ���۸�
		//HDC memoryDC = CreateCompatibleDC(hdc);
		//HBITMAP memoryBitmap = CreateCompatibleBitmap(hdc, MAXHORIZONTAL, MAXVERTICAL);
		//HBITMAP defaultBitmap = (HBITMAP)SelectObject(memoryDC, memoryBitmap);

		//HDC imgDC = CreateCompatibleDC(hdc);
		//HBITMAP hBitmap = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		//	MAKEINTRESOURCE(IDB_BITMAP1));

		//SelectObject(imgDC, hBitmap);

		//// �޸𸮷� �׷��ְ�
		//BitBlt(memoryDC, 0, 0, MAXHORIZONTAL, MAXVERTICAL, imgDC, 0, 0, SRCCOPY);
		//
		////ȭ�鿡 �׷��ְ�
		//BitBlt(hdc, 0, 0, MAXHORIZONTAL, MAXVERTICAL, memoryDC, 0, 0, SRCCOPY);

		//SelectObject(hdc, defaultBitmap);

		//// �����
		//DeleteObject(hBitmap);
		//DeleteDC(imgDC);

		//DeleteObject(memoryBitmap);
		//DeleteDC(memoryDC);

		EndPaint(hWnd, &ps);
		}
	}
		break;
	case WM_LBUTTONDOWN:
	{
		OutputDebugString(L"LBUTTONC\n");
		//int result = MessageBox(NULL, L"���� Ŭ��", L"LEFT", MB_YESNOCANCEL);
		/*if (result == IDYES)
		{
			MessageBox(NULL, L"�� Ŭ��", L"left", MB_YESNO);
		}*/
		// �簢�� �׸��� 

		HDC hdc = GetDC(hWnd);
		makingFigure(hdc);
		ReleaseDC(hWnd, hdc);

	}
		break;
	case WM_RBUTTONDOWN:
		{

		}
		break;
	case WM_LBUTTONDBLCLK:
	{
		OutputDebugString(L"LBUTTONDC\n");
	}
		break;
	case WM_MOUSEMOVE:
		{
			int mouseX = LOWORD(lParam);
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
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



void makingFigure(HDC hdc)
{
	int startX = rand() % MAXHORIZONTAL;
	int startY = rand() % MAXVERTICAL;

	int endX = rand() % MAXHORIZONTAL;
	int endY = rand() % MAXVERTICAL;

	HBRUSH defaultBrush, makingBrush;

	int red = rand() % MAXCOLOR;
	int green = rand() % MAXCOLOR;
	int blue = rand() % MAXCOLOR;

	int style = rand() % MAXSTYLE;

	makingBrush = CreateHatchBrush(style, RGB(red, green, blue));
	defaultBrush = (HBRUSH)SelectObject(hdc, makingBrush);

	if (rand() % 2 == 0)
	{
		Ellipse(hdc, startX, startY, endX, endY);
	}
	else
	{
		Rectangle(hdc, startX, startY, endX, endY);
	}

	DeleteObject(makingBrush);
	SelectObject(hdc, defaultBrush);
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
