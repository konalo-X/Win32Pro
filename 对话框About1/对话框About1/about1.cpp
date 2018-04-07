#include<Windows.h>
//Ä£Ì¬¶Ô»°¿ò--
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("About1");
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, szAppName);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = szAppName;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	
	
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_OK);
		return 0;
	}

	hwnd = CreateWindow(szAppName, 
		TEXT("About Box Demo program"), 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	switch (message)
	{
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_HELP:
			DialogBox(hInstance, TEXT("ABOUT1"), hwnd, DialogProc);
			break;
		default:
			break;
		}
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
	
}
BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParm, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_COMMAND:
			switch (LOWORD(wParm))
			{
			case ID_OK:
				EndDialog(hDlg, 0);
				return TRUE;
				
			}
			break;
			
	}
	return FALSE;
}