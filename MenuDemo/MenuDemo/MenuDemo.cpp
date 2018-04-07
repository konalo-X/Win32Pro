#include<windows.h>
#include "resource.h"
#define ID_TIMER 1
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR  szAppName[] = TEXT("MenuDemo");
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)

{
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
	wndclass.lpszMenuName = szAppName;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This Program Requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("The MenuDemo Program"), WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, NULL);
	
	
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
	static int iSelection = ID_BACKGROUND_WHITE;
	static int idColors[5] = { WHITE_BRUSH,LTGRAY_BRUSH,GRAY_BRUSH,DKGRAY_BRUSH,BLACK_BRUSH };
	static HMENU hMenu;
	switch (message)
	{
	case WM_CREATE:
		 hMenu = GetMenu(hwnd);
		return 0;
	case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_FILE_NEW:
			case ID_FILE_OPEN:
			case ID_FILE_SAVE:
			case ID_FILE_SAVEAS:
			case ID_EDIT_COPY:
			case ID_EDIT_CUT:
			case ID_EDIT_PASTE:
			case ID_EDIT_DELETE:
			case ID_EDIT_UNDO:
				MessageBeep(0);
				return 0;
			case ID_BACKGROUND_WHITE:
			case ID_BACKGROUND_GRAY:
			case ID_BACKGROUND_DARKGRAY:
			case ID_BACKGROUND_LIGHTGRAY:
			case ID_BACKGROUND_BLACK:
				CheckMenuItem(hMenu, iSelection, MF_UNCHECKED);
					iSelection = LOWORD(wParam);
					CheckMenuItem(hMenu, iSelection, MF_CHECKED);
					SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(idColors[iSelection - ID_BACKGROUND_WHITE]));
					InvalidateRect(hwnd, NULL, TRUE);
					return 0;
			case ID_FILE_EXIT:
				SendMessage(hwnd, WM_CLOSE, 0, 0);
				return 0;
			case ID_TIMER_START:
				if (SetTimer(hwnd, ID_TIMER, 1000, NULL))
				{
					EnableMenuItem(hMenu, ID_TIMER_START, MF_GRAYED);
					EnableMenuItem(hMenu, ID_TIMER_STOP, MF_ENABLED);
				}
				
				return 0;
			case ID_TIMER_STOP:
				KillTimer(hwnd, ID_TIMER);
				EnableMenuItem(hMenu, ID_TIMER_STOP, MF_GRAYED);
				EnableMenuItem(hMenu, ID_TIMER_START, MF_ENABLED);
				return 0;
			}
			break;

	case WM_TIMER:
		MessageBeep(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}