#include "resource.h"
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("colorscroll");
	static HWND hDlg;
	WNDCLASS wndclass;
	MSG msg;
	HWND hwnd;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = CreateSolidBrush(0L);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndclass))
		return 0;

	hwnd = CreateWindow(szAppName, TEXT("Color ScrollBars"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);
	hDlg = CreateDialog(hInstance, TEXT("COLORDLG"), hwnd, DlgProc);
	
	while (GetMessage(&msg,NULL,0,0))
	{
		if (hDlg==0||!IsDialogMessage(hDlg,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (message)
	{

	case WM_DESTROY:
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND,(LONG)GetStockObject(WHITE_BRUSH)));
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hScrollBar,hwndParent;
	int iIndex;
	static int icolors[3];
	switch (message)
	{
	case WM_INITDIALOG:
		for (int hCtrlID = 11; hCtrlID < 14; hCtrlID++)
		{
			hScrollBar = GetDlgItem(hDlg, hCtrlID);
			SetScrollRange(hScrollBar, SB_CTL, 0, 255, FALSE);
			SetScrollPos(hScrollBar, SB_CTL, 0, FALSE);
		}
		break;

	case WM_VSCROLL:
		hScrollBar = (HWND)lParam;
		iIndex = GetDlgCtrlID(hScrollBar) - 10;

		switch (LOWORD(wParam))
		{
		case SB_TOP:	
			icolors[iIndex] = 0;
			break;
		case SB_BOTTOM:
			icolors[iIndex] = 255;
			break;
		
		case SB_PAGEDOWN:
			icolors[iIndex] += 15;
		case SB_LINEDOWN:
			icolors[iIndex] =min(255,icolors[iIndex]+1);
			break;
		case SB_PAGEUP:
			icolors[iIndex] -= 15;
		case SB_LINEUP:
			icolors[iIndex] = max(0,icolors[iIndex]-1);
			break;
		case SB_THUMBPOSITION:		
		case SB_THUMBTRACK:	
			icolors[iIndex] = (HIWORD(wParam));
		}
		SetScrollPos(hScrollBar, SB_CTL, icolors[iIndex], TRUE);
		SetDlgItemInt(hDlg, GetDlgCtrlID(hScrollBar) + 3, icolors[iIndex], 0);
		HBRUSH hBrush=CreateSolidBrush(RGB(icolors[0], icolors[1], icolors[2]));
		hwndParent = GetParent(hDlg);
		DeleteObject((HGDIOBJ)SetClassLong(hwndParent, GCL_HBRBACKGROUND, (LONG)hBrush));
		InvalidateRect(hwndParent, NULL, TRUE);
		break;
	}
	return FALSE;
}