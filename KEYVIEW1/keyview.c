#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("KeyView1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	
	if  (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("窗口注册失败!"), szAppName, MB_OK);
		
	}

	hwnd=CreateWindow(szAppName, TEXT("Keyboard Message viewer #1 "), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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

		static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
		static int cLinesMax, cLines;
		static PMSG pmsg;
		static RECT rectScroll;
		static TCHAR szTop[] = TEXT("Message        Key        Char    ")TEXT(" Repeat  Scan  Ext  ALT  Prev  Tran");
		static TCHAR szUnd[] = TEXT("_______        ___        ____     ")      TEXT("______  ____  ___  ___  ____  ____");
		static TCHAR *szFormat[2] = {
			TEXT("%-13s %3d%s%c%19d%7d%7s  %s   %s  %s "),
			TEXT("%-13s            0x%04X%s%c%5d%7d%7s  %s  %s %s") };																																			//TEXT("%-13s %3d%s"),
																																										//TEXT("%-13s          0x%04x%s") };
		static TCHAR *szYes = TEXT("Yes");
		static TCHAR *szNo = TEXT("No");
		static TCHAR *szDown = TEXT("Down");
		static TCHAR *szUp = TEXT("Up");

		static TCHAR *szMessage[] = {
			TEXT("WM_KEYDOWN"),TEXT("WM_KEYUP"),
			TEXT("WM_CHAR"),TEXT("WM_DEADCHAR"),
			TEXT("WM_SYSKEYDOWN"),TEXT("WM_SYSKEYUP"),
			TEXT("WM_SYSCHAR"),TEXT("WM_SYSDEADCHAR")
		};

		HDC hdc;
		int i, iType;
		PAINTSTRUCT ps;
		static TCHAR szBuffer[128], szBuffer1[128],szKeyName[32];
		TEXTMETRIC tm;

		switch (message)
		{
		case WM_CREATE:
		case WM_DISPLAYCHANGE:

			//get maxsize of client area
			cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
			cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);
			//get character size for fixed-pitch font(固定宽度)
			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight;
			ReleaseDC(hwnd, hdc);

			//allocate memory for display lines

			if (pmsg != NULL)
			{
				free(pmsg);
			}
			cLinesMax = cyClientMax / cyChar;
			pmsg = malloc(cLinesMax * sizeof(MSG));
			cLines = 0;

		case WM_SIZE:
			if (message == WM_SIZE)
			{
				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
			}
			//计算滚动矩形
			rectScroll.left = 0;
			rectScroll.right = cxClient;
			rectScroll.top = cyChar;
			rectScroll.bottom = cyChar*(cyClient / cyChar);
			InvalidateRect(hwnd, &rectScroll, TRUE);
			return 0;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
		case	WM_SYSDEADCHAR:

			//rearrange storage array
			for (i = cLinesMax - 1; i > 0; i--)
			{
				pmsg[i] = pmsg[i - 1];
			}

			pmsg[0].hwnd = hwnd;
			pmsg[0].lParam = lParam;
			pmsg[0].message = message;
			pmsg[0].wParam = wParam;

			cLines = min(cLines + 1, cLinesMax);
			//scrollup the display
			ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);

			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
			TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));

			for (i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
			{
				iType = pmsg[i].message == WM_CHAR ||
					pmsg[i].message == WM_SYSCHAR ||
					pmsg[i].message == WM_DEADCHAR ||
					pmsg[i].message == WM_SYSDEADCHAR;

				GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));

				TextOut(hdc, 0, (cyClient / cyChar - i - 1)*cyChar, szBuffer,
					wsprintf(szBuffer, szFormat[iType],
						szMessage[pmsg[i].message - WM_KEYFIRST],						//确定数组第几个元素
						pmsg[i].wParam,																// 包含ASCII 和UNICODE(可以确切知道按中的是字母大小写)
						(PTSTR)(iType ? TEXT(" ") : szKeyName),					// 按键名字-->>WM_KEYUP&WM_KEYDOWN~~ 会显示
						(TCHAR)(iType ? pmsg[i].wParam : ' '),
					LOWORD(pmsg[i].lParam),//16位重复计数>1代表程序处理速度小于输入速度
					HIWORD(pmsg[i].lParam) & 0xFF,//0000000011111111&高16位得出8位扫描码
					0x01000000 & pmsg[i].lParam ? szYes : szNo,//拓展标记
					0x20000000 & pmsg[i].lParam ? szYes : szNo,
					0x40000000 & pmsg[i].lParam ? szDown : szUp,
					0x80000000 & pmsg[i].lParam ? szUp : szDown));
					
			}
		/*
				TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
					wsprintf(szBuffer, szFormat[iType],
						szMessage[pmsg[i].message - WM_KEYFIRST],
						pmsg[i].wParam,
						(PTSTR)(iType ? TEXT(" ") : szKeyName),
						(TCHAR)(iType ? pmsg[i].wParam : ' '),
						LOWORD(pmsg[i].lParam),
						HIWORD(pmsg[i].lParam) & 0xFF,
						0x01000000 & pmsg[i].lParam ? szYes : szNo,
						0x20000000 & pmsg[i].lParam ? szYes : szNo,
						0x40000000 & pmsg[i].lParam ? szDown : szUp,
						0x80000000 & pmsg[i].lParam ? szUp : szDown));
			}
*/
			EndPaint(hwnd, &ps);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}


	return DefWindowProc(hwnd,message,wParam,lParam);
}