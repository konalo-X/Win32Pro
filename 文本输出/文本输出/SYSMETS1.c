
#define WINVER 0x0500
#include<Windows.h>
#include"SYSMETS.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND hwnd;
	MSG    msg;
	WNDCLASS wndclass;
	static TCHAR szName[] = TEXT("SysMets1");
	
	/*wndclass*/
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("无法注册窗口类1!"), szName, MB_OK|MB_ICONERROR);
	}


	hwnd=CreateWindow(
		szName,
		TEXT("Get Systems Metrics No.1"),
		WS_OVERLAPPEDWINDOW|WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hwnd, iCmdShow);
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
	static int cxChar, cxCaps, cyChar, cyClient, cxClient, maxline, ivscrollpos;
	 static HDC  hdc;
	PAINTSTRUCT ps;
	int i,y;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;
	
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;//加权平均宽度,一般是小写字符1.5倍
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);

		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hwnd, SB_VERT, ivscrollpos, TRUE);

		return 0;
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		//cxClient = LOWORD(lParam);
		//maxline = cyChar / cyChar;
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			ivscrollpos -= 1;
			break;
		case SB_LINEDOWN:
			ivscrollpos += 1;
			break;
		case SB_PAGEDOWN:
			ivscrollpos += cyClient / cyChar;
			break;
		case SB_PAGEUP:
			ivscrollpos -= cyClient / cyChar;
			break;
		default:
			break;
		}
	
		ivscrollpos = max(0, min(ivscrollpos, NUMLINES - 1));//?
		
		if (ivscrollpos != GetScrollPos(hwnd, SB_VERT))
		{
			SetScrollPos(hwnd, SB_VERT, ivscrollpos, TRUE);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		for ( i = 0; i < NUMLINES; i++)
		{
			y = cyChar*(i - ivscrollpos);
			TextOut(hdc, 0, y, sysmetrics[i].szLabel, wcslen(sysmetrics[i].szLabel));
			TextOut(hdc, 22*cxCaps, y, sysmetrics[i].szDesc, wcslen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 22 * cxCaps + 44 * cxCaps, y, szBuffer,wsprintf(szBuffer,TEXT("%d"), GetSystemMetrics(sysmetrics[i].iIndex)));//
			TextOut(hdc, 22 * cxCaps + 48 * cxCaps, y, szBuffer, wsprintf(szBuffer, TEXT("%d"), ivscrollpos));//
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}
