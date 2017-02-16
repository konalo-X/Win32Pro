#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInsatnce, PSTR szCmdLine, int iCmdSHow)
{
	static TCHAR szAppName[] = TEXT("BlokOut1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL,  TEXT("Program requires windows NT!"), szAppName ,MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Mouse Button Demo"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdSHow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void  DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd)
{
	HDC hdc;
	hdc = GetDC(hwnd);
	SetROP2(hdc, R2_NOT);						//Specifies the new drawing mode 画图模式
	/*
	R2_BLACK   Pixel is always black.

	R2_WHITE   Pixel is always white.

	R2_NOP   Pixel remains unchanged.

	R2_NOT   Pixel is the inverse of the screen color.//颜色反转

	R2_COPYPEN   Pixel is the pen color.

	R2_NOTCOPYPEN   Pixel is the inverse of the pen color.

	R2_MERGEPENNOT   Pixel is a combination of the pen color and the inverse of the screen color (final pixel = (NOT screen pixel) OR pen).

	R2_MASKPENNOT   Pixel is a combination of the colors common to both the pen and the inverse of the screen (final pixel = (NOT screen pixel) AND pen).

	R2_MERGENOTPEN   Pixel is a combination of the screen color and the inverse of the pen color (final pixel = (NOT pen) OR screen pixel).

	R2_MASKNOTPEN   Pixel is a combination of the colors common to both the screen and the inverse of the pen (final pixel = (NOT pen) AND screen pixel).

	R2_MERGEPEN   Pixel is a combination of the pen color and the screen color (final pixel = pen OR screen pixel).

	R2_NOTMERGEPEN   Pixel is the inverse of the R2_MERGEPEN color (final pixel = NOT(pen OR screen pixel)).

	R2_MASKPEN   Pixel is a combination of the colors common to both the pen and the screen (final pixel = pen AND screen pixel).

	R2_NOTMASKPEN   Pixel is the inverse of the R2_MASKPEN color (final pixel = NOT(pen AND screen pixel)).

	R2_XORPEN   Pixel is a combination of the colors that are in the pen or in the screen, but not in both (final pixel = pen XOR screen pixel).

	R2_NOTXORPEN   Pixel is the inverse of the R2_XORPEN color (final pixel = NOT(pen XOR screen pixel)).


	
	
	*/
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
	ReleaseDC(hwnd, hdc);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL fBlocking, fValidBox;
	static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
		
	{
	case WM_LBUTTONDOWN:
		ptBeg.x = ptEnd.x = LOWORD(lParam);
		ptBeg.y = ptEnd.y = HIWORD(lParam);

		DrawBoxOutline(hwnd, ptBeg, ptEnd);
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		fBlocking = TRUE;
		return 0;

	case WM_MOUSEMOVE:
		if (fBlocking)		// 确保点击鼠标左键后才会划线
		{
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptEnd.x = LOWORD(lParam);
			ptEnd.y = HIWORD(lParam);

			DrawBoxOutline(hwnd, ptBeg, ptEnd);
		}
		return 0;

	case WM_LBUTTONUP:
		if (fBlocking)
		{
			DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptBoxBeg = ptBeg;
			ptBoxEnd.x = LOWORD(lParam);
			ptBoxEnd.y = HIWORD(lParam);
		//	ptBoxEnd = ptEnd;

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = FALSE;
			fValidBox = TRUE;

			InvalidateRect(hwnd, NULL, TRUE);

		}
		return 0;
	case WM_CHAR:
		if (fBlocking&(wParam == '\x1B'))
		{
			DrawBoxOutline(hwnd, ptBeg, ptEnd);

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = FALSE;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if (fValidBox)
		{
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);

		}
		if (fBlocking)
		{
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		
	}return DefWindowProc(hwnd, message, wParam, lParam);
}