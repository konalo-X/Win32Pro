#include <windows.h>
#include <tchar.h>

/*
EnumDisplaySettings 函数,查询当前显示器所有显示参数
		while (EnumDisplaySettings(NULL,iDevMode++,&DevMode))
	{
		*pxBitMap = max(*pxBitMap, DevMode.dmPelsWidth);
		*pyBitmap = max(*pyBitmap, DevMode.dmPelsHeight);
	}

	hdcMem = CreateCompatibleDC(hdc);
	getLargestDisplay(&cxBitmap, &cyBitmap);
	hBitmap = CreateCompatibleBitmap(hdc, cxBitmap, cyBitmap);


	cxMouse =(short) LOWORD(lParam);
	cyMouse = (short)HIWORD(lParam);

	BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
	画白色矩形
	PatBlt(hdcMem, 0, 0, cxBitmap, cyBitmap, WHITENESS);
	
*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("SKETCH DEMO");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = 0;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires windows nt!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, hInstance, NULL);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void getLargestDisplay(int *pxBitMap,int *pyBitmap)
{
	DEVMODE DevMode;
	DWORD iDevMode=0;
	*pxBitMap = *pyBitmap = 0;
	DevMode.dmSize = sizeof(DEVMODE);
	ZeroMemory(&DevMode, sizeof(DEVMODE));
	while (EnumDisplaySettings(NULL,iDevMode++,&DevMode))
	{
		*pxBitMap = max(*pxBitMap, DevMode.dmPelsWidth);
		*pyBitmap = max(*pyBitmap, DevMode.dmPelsHeight);
	}

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HDC hdcMem;
	static HBITMAP hBitmap;
	static int cxClient, cyClient, cxBitmap, cyBitmap,cxMouse,cyMouse;
	int x, y;
	static BOOL fLeftButtonDown, fRightButtonDown;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
	
		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		getLargestDisplay(&cxBitmap, &cyBitmap);
		hBitmap = CreateCompatibleBitmap(hdc, cxBitmap, cyBitmap);
		if (!hBitmap)
		{
			DeleteObject(hdcMem);
			return -1;
		}
		SelectObject(hdcMem, hBitmap);
		//BitBlt(hdc, 0, 0, cxBitmap, cyBitmap, hdcMem, 0, 0, WHITENESS);
		PatBlt(hdcMem, 0, 0, cxBitmap, cyBitmap, WHITENESS);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_LBUTTONDOWN:
	//	if(!fRightButtonDown)
			SetCapture(hwnd);
		cxMouse = LOWORD(lParam);
		cyMouse = HIWORD(lParam);
		fLeftButtonDown = TRUE;

		return 0;

	case WM_LBUTTONUP:
	//	if(fLeftButtonDown)
			SetCapture(NULL);
		fLeftButtonDown = FALSE;
	
		return 0;

	case WM_RBUTTONDOWN:
	//	if(!fLeftButtonDown)
				SetCapture(hwnd);
		cxMouse = LOWORD(lParam);
		cyMouse = HIWORD(lParam);
		fRightButtonDown = TRUE;
		hdc = GetDC(hwnd);
		//MoveToEx(hdc, cxMouse, cyMouse, NULL);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_RBUTTONUP:
	//	if (fRightButtonDown)
			SetCapture(NULL);
		fRightButtonDown = FALSE;
		
		return 0;

	case WM_MOUSEMOVE:
		if (!fLeftButtonDown && !fRightButtonDown)
			return 0;
		
		hdc = GetDC(hwnd);
		if (fLeftButtonDown)
		{
			SelectObject(hdc, GetStockObject(BLACK_PEN));
		}
		if (fRightButtonDown)
		{
			SelectObject(hdc, GetStockObject(WHITE_PEN));
		}

		MoveToEx(hdc, cxMouse, cyMouse, NULL);
		MoveToEx(hdcMem, cxMouse, cyMouse, NULL);
		cxMouse =(short) LOWORD(lParam);
		cyMouse = (short)HIWORD(lParam);
		LineTo(hdc, cxMouse, cyMouse);
		LineTo(hdcMem, cxMouse, cyMouse);
			ReleaseDC(hwnd, hdc);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:

		
		DeleteObject(hBitmap);
		DeleteObject(hdcMem);
		PostQuitMessage(0);
		return 0;
		
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}