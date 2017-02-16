#include<windows.h>
#include"DEVCAPS1.h"
/*define a windproc*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("DevCaps1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	/*设置wndclass 各个属性*/
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = WS_OVERLAPPED|CS_HREDRAW|CS_VREDRAW|CS_OWNDC;

	/*注册窗口类*/
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Register class fail!"), szAppName, MB_ICONERROR);
		return 0;
	}
	/*创建窗口*/
	hwnd = CreateWindow(szAppName,
		TEXT("This program requires Windows NT!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	/*显示窗口*/

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	/*分发消息*/

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;//当前窗口消息wParm
}
/*窗口过程函数的实现*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	static int cxChar, cyChar, cxCaps,iSavedDC;
	TCHAR szBuffer[10];
	int i;

	switch (message)
	{
	case WM_CREATE:
		hdc=GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmExternalLeading + tm.tmHeight;
		iSavedDC=SaveDC(hdc);
		//SetBkColor(hdc, TEXT("0x00ffeeaa"));
		SaveDC(hdc);
		ReleaseDC(hwnd, hdc);
		return 0;
		
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for ( i = 0; i < NUMLINES; i++)
		{
			
			TextOut(hdc, 0, cyChar*i, devcaps[i].szLabel, wcslen(devcaps[i].szLabel));
		//	RestoreDC(hdc, -1);
			TextOut(hdc, 13 * cxCaps, cyChar*i, devcaps[i].szDesc, wcslen(devcaps[i].szDesc));
			SetTextAlign(hdc, TA_TOP | TA_RIGHT);
			//SetBkColor(hdc, TEXT("0x00eeeeee"));
			TextOut(hdc, 13 * cxCaps + 45 * cxCaps, cyChar*i,szBuffer,wsprintf(szBuffer, TEXT("%5d"),GetDeviceCaps(hdc,devcaps[i].iIndex)));
			SetTextAlign(hdc, TA_TOP | TA_LEFT);

		
		}
		/*for (i = 0; i <400; i++)
		{
			画一条竖直线
			SetPixel(hdc, 100 + i, i, TEXT("0x00eeeeee"));
		}*/
		/*
		MoveToEx(hdc, 600, 800, NULL);
		LineTo(hdc, 300, 100);
		EndPaint(hwnd, &ps);*/
		/*在客户区画一条网格*/
		//RECT rec;
		//GetClientRect(hwnd, &rec);
		//for (int x = 0; x < rec.right; x+=100)
		//{
		//	MoveToEx(hdc, x, 0, NULL);
		//	LineTo(hdc, x, rec.bottom);
		//}
		//for (int y = 0; y < rec.right; y+=100)
		//{
		//	MoveToEx(hdc, 0, y, NULL);
		//	LineTo(hdc, rec.right, y);

		//}
		POINT apt[5] = { 100,100,200,100,200,200,100,200,100,100 };
		MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
		//for (int i = 0; i < 5; i++)
	/*	{
			LineTo(hdc, 100, 300);
			LineTo(hdc, 200, 600);
		}*/
		//Polyline(hdc, apt, 5);
		PolylineTo(hdc, apt , 5);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}