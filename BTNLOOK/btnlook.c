#include<windows.h>
#define NUM sizeof button/sizeof button[0]
struct 
{
	int btnStyle;
	TCHAR * szText;
}button[]={
	BS_PUSHBUTTON,					TEXT("BS_PUSHBUTTON"),
	BS_CHECKBOX,						TEXT("BS_CHECKBOX"),
	BS_AUTOCHECKBOX,				TEXT("BS_AUTOCHECKBOX"),
	BS_RADIOBUTTON,					TEXT("BS_RADIOBUTTON"),
	BS_3STATE,								TEXT("BS_3STATE"),
	BS_AUTO3STATE,					TEXT("BS_AUTO3STATE"),
	BS_GROUPBOX,						TEXT("BS_GROUPBOX"),
	BS_USERBUTTON,					TEXT("BS_USERBUTTON"),
	BS_AUTORADIOBUTTON,		TEXT("BS_AUTORADIOBUTTON"),
	BS_PUSHBOX,							TEXT("BS_PUSHBOX"),
	BS_OWNERDRAW,					TEXT("BS_OWNERDRAW")
};
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsatnce, PSTR szCmdLine, int iCmdShow)
{
	HWND hwnd;
	WNDCLASS wndclass;
	MSG msg;
	TCHAR szAppName[] = TEXT("BTNLOOK APP");

	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName =szAppName;
	wndclass.lpszMenuName = NULL;
	
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Window Class registe fail!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Button look App"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

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
	HWND childhwnd[NUM];
	HDC hdc;
	int i, cxClient, cyClient;
	static RECT rect;
	static int cxChar, cyChar;
	PAINTSTRUCT ps;
	static TCHAR szTop[] = TEXT("message            wparam             lparam");
	static TCHAR	szUnd[] = TEXT("_______            ______             ______");
	 TCHAR szBuffer[50];
	TCHAR szFormat[] = TEXT("%-21s %04x-%04x     %04x-%04x");
	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		for ( i = 0; i < NUM; i++)
		{
			childhwnd[i] = CreateWindow(
				TEXT("button"), button[i].szText, WS_CHILD | WS_VISIBLE | button[i].btnStyle,
				cxChar, cyChar, 24 * cxChar, 7 * cyChar / 4, hwnd, (HMENU)i, ((LPCREATESTRUCT)lParam)->hInstance, NULL
			);
			MoveWindow(childhwnd[i], cxChar, cyChar*(2 * i + 1), 24 * cxChar, 7 * cyChar / 4, TRUE);
		}
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		rect.top = 2*cyChar;
		rect.left = 26 * cxChar;
		rect.right = cxClient;
		rect.bottom = cyClient;
		return 0;
	case WM_DRAWITEM:
	case WM_COMMAND:
	ScrollWindow(hwnd, 0, -cyChar, &rect, &rect);//scrollwindow 会产生wm_paint消息
		hdc = GetDC(hwnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		TextOut(hdc, 26 * cxChar, rect.bottom - cyChar, szBuffer, wsprintf(szBuffer, szFormat,
			(message==WM_DRAWITEM)?TEXT("WM_DRAWITEM"):TEXT("WM_COMMAND"),
			HIWORD(wParam),LOWORD(wParam),
			HIWORD(lParam),LOWORD(lParam)));
		ReleaseDC(hwnd, hdc);
		ValidateRect(hwnd, &rect);
		return 0;
	case WM_PAINT:
		InvalidateRect(hwnd, &rect, TRUE);
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc, 26 * cxChar, cyChar, szTop, lstrlen(szTop));
		TextOut(hdc, 26 * cxChar, cyChar, szUnd, lstrlen(szUnd));
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}