#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("BitBlt");
	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	
	if (!RegisterClass(&wndclass	))
	{
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("BitBlt Demo"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd,nShowCmd);
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
	static int cxClient, cyClient, cxSource, cySource;
	HDC hdcClient,hdcWindow;
	int x, y;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		cxSource = GetSystemMetrics(SM_CXSIZEFRAME)+ GetSystemMetrics(SM_CXSIZE);
		cySource = GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION);
		return 0;
	case WM_SIZE:
		cxClient =LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdcClient = BeginPaint(hwnd, &ps);
		hdcWindow = GetWindowDC(hwnd);
	//	for (y=0;y<cyClient;y+=cySource)
		//	for (	 x = 0; x < cxClient; x+=cxSource)
			{
				BitBlt(hdcClient, 0, 0, cxSource, cySource, hdcWindow, 0, 0, SRCCOPY);
			}
	
		ReleaseDC(hwnd, hdcWindow);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
