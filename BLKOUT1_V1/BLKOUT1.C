#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsatnce, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("BLKOUT1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("The Program requires windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Mouse Button  Demo"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void DrawBoxOutLine(POINT ptBeg, POINT ptEnd,HWND hwnd)
{
	HDC hdc=GetDC(hwnd);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT ptBeg, ptEnd,ptBoxBeg,ptBoxEnd;
	static BOOL fBOXLine,fBox;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CHAR:
		if (wParam == '\x1B')
		{
			fBOXLine = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
		}

		return 0;
	case WM_LBUTTONDOWN:
		SetCapture(hwnd);
		ptBeg.x =ptBoxBeg.x= LOWORD(lParam);
		ptBeg.y =ptBoxBeg.y= HIWORD(lParam);
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		fBOXLine = TRUE;
		fBox = FALSE;
		return 0;

	case WM_MOUSEMOVE:
		ptEnd.x = LOWORD(lParam);
		ptEnd.y = HIWORD(lParam);

		if (fBOXLine)
		{
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			//DrawBoxOutLine(ptBeg, ptEnd, hwnd);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		
		return 0;

	case WM_LBUTTONUP:
		ptEnd.x = ptBoxEnd.x = LOWORD(lParam);
		ptEnd.y = ptBoxEnd.y = HIWORD(lParam);

		//DrawBoxOutLine(ptBeg, ptEnd, hwnd);
		InvalidateRect(hwnd, NULL, TRUE);
		ReleaseCapture();
		fBOXLine = FALSE;
		fBox = TRUE;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (fBOXLine)
		{
				DrawBoxOutLine(ptBeg, ptEnd, hwnd);
		}
		if (fBox)
		{
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
		}

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}


	return DefWindowProc(hwnd, message, wParam, lParam);
}