#include<windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR  szAppName[] = TEXT("PopMenu");


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
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This Program Requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("The PopMenuDemo Program"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);


	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
HINSTANCE hinst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int x, y;
	POINT pt;
	static int iSelection = ID_BACKGROUND_WHITE;
	static HMENU hMenu;
	int iColors[] = {
		WHITE_BRUSH,
		LTGRAY_BRUSH,
		GRAY_BRUSH,
		DKGRAY_BRUSH,
		BLACK_BRUSH
	};
	switch (message)
	{
	case WM_CREATE:
		hMenu=LoadMenu(hinst, szAppName);
		hMenu = GetSubMenu(hMenu, 0);
		return 0;
	case WM_RBUTTONDOWN:
		 
		 
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		ClientToScreen(hwnd, &pt);
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
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
			break;
		case ID_BACKGROUND_WHITE:
		case ID_BACKGROUND_DARKGRAY:
		case ID_BACKGROUND_GRAY:
		case ID_BACKGROUND_LIGHTGRAY:
		case ID_BACKGROUND_BLACK:
			CheckMenuItem(hMenu, iSelection, MF_UNCHECKED);
			iSelection = LOWORD(wParam);
			CheckMenuItem(hMenu, iSelection, MF_CHECKED);
			SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(iColors[iSelection - ID_BACKGROUND_WHITE]));
			InvalidateRect(hwnd, NULL, TRUE);
			
				break;
		case ID_FILE_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		
			
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}