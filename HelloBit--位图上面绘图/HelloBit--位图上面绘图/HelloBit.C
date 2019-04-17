#include "resource.h"
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wndClass;
	HMENU hMenu;
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
	static TCHAR szAppName[] = TEXT("HelloBit Demo");


	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("this program requires wiindows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Hellobit Demo--位图上绘图"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, hMenu,hInstance,NULL);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);


	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hinst;
	static HMENU hMenu;
	static BOOL bEnable=TRUE;
	static HBITMAP hBitmap;
	static HDC hdcMem;
	static int cxBitmap, cyBitmap, cxClient, cyClient, iSize = ID_SIZE_BIG;
	static TCHAR * szText = TEXT("Hello,world! ");
	HDC hdc;
	int x, y;
	PAINTSTRUCT ps;
	SIZE size;//结构体size{cx,cy}
	switch (message)
	{
	case WM_CREATE:

		//加载menu的一种方法
		////hinst = ((LPCREATESTRUCT)lParam)->hInstance;
		////hMenu=LoadMenu(hinst, MAKEINTRESOURCE(IDR_MENU1));
		////SetMenu(hwnd, hMenu);
		hMenu = ((LPCREATESTRUCT)lParam)->hMenu;
		hdc = GetDC(hwnd);//获得客户区DC
		hdcMem = CreateCompatibleDC(hdc);//获得与hdc相互兼容的内存设备环境
		//确定文本字符串的像素大小
		GetTextExtentPoint32(hdc, szText, lstrlen(szText), &size);//The GetTextExtentPoint32 function computes the width and height of the specified string of text. in logical units. 赋值给size
		
		cxBitmap = size.cx;
		cyBitmap = size.cy;
		
		hBitmap=CreateCompatibleBitmap(hdc, cxBitmap, cyBitmap);//CreateCompatibleBitmap函数创建一个与与指定设备环境兼容的位图。

		ReleaseDC(hwnd, hdc);
		//加载bitmap到内存设备环境
		SelectObject(hdcMem, hBitmap);
		TextOut(hdcMem, 0, 0, szText, lstrlen(szText));
		return 0;
	case WM_INITMENUPOPUP:
		CheckMenuItem(hMenu, ID_SIZE_BIG, bEnable ? MF_CHECKED : MF_UNCHECKED);
		//CheckMenuItem(hMenu, ID_SIZE_BIG, MF_ENABLED );
		//EnableMenuItem搞混了
		CheckMenuItem(hMenu, ID_SIZE_SMALL, bEnable ? MF_UNCHECKED : MF_CHECKED);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SIZE_SMALL:
			bEnable = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case ID_SIZE_BIG:
			bEnable = TRUE;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		switch (bEnable)
		{
		case 1:
			StretchBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, cxBitmap, cyBitmap, SRCCOPY);
			break;
		case 0:
			for (x=0;x<cxClient;x+=cxBitmap)
				for(y=0;y<cyClient;y+=cyBitmap)
					BitBlt(hdc, x, y, cxBitmap, cyBitmap, hdcMem, 0, 0, SRCCOPY);
			break;
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CLOSE:
		DeleteObject(hdcMem);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}