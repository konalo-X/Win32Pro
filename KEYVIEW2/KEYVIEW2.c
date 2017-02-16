#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("KeyView2");
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
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires WindowsNT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("KeyBoard Message Viewer#2"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD dwCharSet = DEFAULT_CHARSET;
	static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
	static int cLinemax, Clines;
	static PMSG pmsg;
	static RECT rectscroll;
	static TCHAR szTop[] = TEXT("Message Key Char")
		TEXT("Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______ ___ ____")
		TEXT("______ ____ ___ ___ ____ ____");
	static TCHAR *szFormat[2] = {
		TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
		TEXT("%-13s 0x%04X%1s%c %6u %4d %3s %3s %4s %4s") };
	static TCHAR * szYes = TEXT("yes");
	static TCHAR * szNo = TEXT("No");
	static TCHAR * szDown = TEXT("Down");
	static TCHAR * szUp = TEXT("Up");

	static TCHAR * szMessage[] = {
		TEXT("WM_KEYDOWN"),TEXT("WM_KEYUP"),
		TEXT("WM_CHAR"),TEXT("WM_DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"),TEXT("WM_SYSKEYUP"),
		TEXT("WM_SYSCHAR"),TEXT("WM_SYSDEADCHAR") };

	HDC hdc;
	int i, iType;
	PAINTSTRUCT ps;
	TCHAR szBuffer[128], szKeyName[32];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_INPUTLANGCHANGE:
		dwCharSet = wParam;

	case WM_CREATE:
	case WM_DISPLAYCHANGE:

		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);

		hdc = GetDC(hwnd);
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		ReleaseDC(hwnd, hdc);

		if (pmsg)
		{
			free(pmsg);
		}
		cLinemax = cyClientMax / cyChar;
		pmsg = malloc(cLinemax*sizeof(MSG));
		Clines = 0;

	case WM_SIZE:
		if (message==WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
		/*rectscroll.left = 0;
		rectscroll.right = cxClient;
		rectscroll.top = cyChar+1;
		rectscroll.bottom = cyChar*(cyClient / cyChar);//------------------------------------------------------------
		*/
		GetClientRect(hwnd, &rectscroll);
		rectscroll.top = cyChar;
		rectscroll.bottom = (cyClient/cyChar)*cyChar;
		InvalidateRect(hwnd, &rectscroll, TRUE);

		if (message==WM_INPUTLANGCHANGE)
		{
			return TRUE;
		}
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:

		for (i=cLinemax-1;i>0;i--)
		{
			pmsg[i] = pmsg[i - 1];
		}						//store new message

		pmsg[0].hwnd = hwnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		Clines = min(Clines + 1, cLinemax);

		ScrollWindow(hwnd, 0, -cyChar, &rectscroll, &rectscroll);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 000, 0, szUnd, lstrlen(szUnd));

		for (i=0;i<min(Clines,cyClient/cyChar-1);i++)
		{
			iType=pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR;

			GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));

			TextOut(hdc, 0, (cyClient / cyChar - 1 - i)*cyChar, szBuffer, wsprintf(szBuffer, szFormat[iType], szMessage[pmsg[i].message - WM_KEYFIRST], pmsg[i].wParam,
				(PTSTR)(iType ? TEXT(" ") : szKeyName),
				(TCHAR)(iType ? pmsg[i].wParam : ' '),
				LOWORD(pmsg[i].lParam),
				HIWORD(pmsg[i].lParam) & 0xFF,
				0x01000000 & pmsg[i].lParam ? szYes : szNo,
				0x20000000 & pmsg[i].lParam ? szYes : szNo,
				0x40000000 & pmsg[i].lParam ? szDown : szUp,
				0x80000000 & pmsg[i].lParam ? szUp : szDown));
		}
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}