#include<Windows.h>
#include"D:\Projects\文本输出\文本输出\SYSMETS.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass;
	HWND hwnd;
	TCHAR szAppName[] = TEXT("Windows !");


	/*初始化窗口类*/
	wndclass.hInstance = hPreInstance;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	/*注册窗口类*/
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("wndclass register fail!"), TEXT("NOTICE!"), MB_OK);
		return 0;
	}

	/*创建窗口*/
	hwnd = CreateWindow(
		szAppName,
		TEXT("HELLOWIN"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	/*显示窗口*/
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);//更新窗口

					   /*消息处理*/
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParm, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rec;
	static int  cyClient, cyChar, cxChar, ivscrollpos;
	TEXTMETRIC tm;
	 ULONG ulScrollLines;
	static	int  iAccumDelta,iDeltaPerLine;
	int i, y;

	switch (message)
	{

	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cxChar = (tm.tmPitchAndFamily & 1 ? 3 : 2)*tm.tmAveCharWidth / 2;//tmPitchAndFamily  低位字段 1:变宽       0 等宽
		TCHAR szBuffer[10];
		ReleaseDC(hwnd, hdc);

		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hwnd, SB_VERT, ivscrollpos, TRUE);

	case WM_SETTINGCHANGE:
		//BOOL WINAPI SystemParametersInfo(
		//	_In_    UINT  uiAction,
		//	_In_    UINT  uiParam,
		//	_Inout_ PVOID pvParam,
		//	_In_    UINT  fWinIni
		//);


		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
		if (ulScrollLines)		//一般滚动的时候默认为3,未滚动的时候值为0
		{
			iDeltaPerLine = WHEEL_DELTA / ulScrollLines;//表示每一行的变化量
		}
		else
			iDeltaPerLine = 0;
		return 0;
		
	case WM_SIZE:
		cyClient = HIWORD(lParam);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rec);
		for (i = 0; i < NUMLINES; i++)
		{
			y = cyChar*(i - ivscrollpos);//当y等于负,不显示
			TextOut(hdc, 0, y, sysmetrics[i].szLabel, wcslen(sysmetrics[i].szLabel));
			TextOut(hdc, 24 * cxChar, y, sysmetrics[i].szDesc, wcslen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 22 * cxChar + 44 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT(" %d"), GetSystemMetrics(sysmetrics[i].iIndex)));
			TextOut(hdc, 22 * cxChar + 50 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT(" ivs=%d"), ivscrollpos));
			TextOut(hdc, 22 * cxChar + 54 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT(" i=%d"), i));
			TextOut(hdc, 22 * cxChar + 59 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT(" cha=%d"), i - ivscrollpos));
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return 0;
	/*case WM_KEYDOWN:
		switch (wParm)
		{
		case VK_HOME:
			SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
			if (GetKeyState(VK_HOME) <= 0)
			{
				MessageBox(NULL, TEXT("HOME was clicked!!"), TEXT("Notice!"), MB_OK);
			}
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_PRIOR:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;
		case VK_NEXT:
			SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;
		case VK_END:
			SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;
		case VK_UP:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			break;
		case VK_DOWN:
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;
		default:
			break;
		}
		return 0;*/
	case WM_MOUSEWHEEL:
		if (iDeltaPerLine == 0)
		{
			break;
		}
		iAccumDelta += (short)HIWORD(wParm);//+120  或者-120
		while (iAccumDelta >= iDeltaPerLine)
		{
			SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
			iAccumDelta -= iDeltaPerLine;
		}
		while (-iAccumDelta >= iDeltaPerLine)
		{
			SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
			iAccumDelta += iDeltaPerLine;
		}
		return 0;
	case WM_VSCROLL:

		switch (LOWORD(wParm))
		{

		case SB_LINEDOWN:
			ivscrollpos += 1;
			break;
		case SB_LINEUP:
			ivscrollpos -= 1;
			break;
		case SB_PAGEDOWN:
			ivscrollpos += cyClient / cyChar;
			break;
		case SB_PAGEUP:
			ivscrollpos -= cyClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			ivscrollpos = HIWORD(wParm);
			break;
		default:
			break;

		}
		ivscrollpos = max(0, min(ivscrollpos, NUMLINES - 1));

		if (ivscrollpos != GetScrollPos(hwnd, SB_VERT))
		{
			SetScrollPos(hwnd, SB_VERT, ivscrollpos, TRUE);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParm, lParam);
}