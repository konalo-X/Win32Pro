

#include<windows.h>
#include<tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT(".ClipView");
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
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("注册失败"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Simple clipboard viewer (Text only)"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd, nCmdShow);
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
	static HWND hwndNextViewer;//保存下一个窗口查看器句柄
	HGLOBAL hGlobal;
	HDC hdc;
	PTSTR pGlobal;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		hwndNextViewer=SetClipboardViewer(hwnd);//当前窗口加入剪贴板查看器链,返回下一个窗口查看器句柄给hwndNextViewer(开始为NULL)
		return 0;

	case WM_CHANGECBCHAIN://程序从剪贴板查看器退出,会收到这个消息
		if ((HWND)wParam == hwndNextViewer)//要移除的直接是当前句柄的下一个
		{
			hwndNextViewer = (HWND)lParam;
		}
		else if (hwndNextViewer)//要移除的隔了几个
			SendMessage(hwndNextViewer, message, wParam, lParam);
		return 0;

	case WM_DRAWCLIPBOARD:
		if (hwndNextViewer)
		{
			SendMessage(hwndNextViewer, message, wParam, lParam);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hwnd);
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_CLICKACTIVE|WA_ACTIVE)
			InvalidateRect(lParam, NULL, TRUE);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		OpenClipboard(hwnd);
#ifdef UNICODE
		hGlobal = GetClipboardData(CF_UNICODETEXT);
#else
		hGlobal = GetClipboardData(CF_TEXT);
#endif // UNICODE
		pGlobal = GlobalLock(hGlobal);
		if (pGlobal)
		{
			DrawText(hdc, pGlobal, -1, &rect, DT_WORDBREAK | DT_EXPANDTABS);
			GlobalUnlock(hGlobal);

		}
		CloseClipboard();
		EndPaint(hwnd, &ps);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}