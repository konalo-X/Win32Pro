#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogProc(HWND ,UINT ,WPARAM ,LPARAM );

void PaintWindow(HWND, int, int);
void PaintBlock(HWND, int, int);
TCHAR szAppName[] = TEXT("About2");
int iCurrentColor = IDC_RADIO1;
int  iCurrentFigure = IDC_RADIO9;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hwnd;
	MSG msg;
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
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("The Program Requires Winodws NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("About2 Program Demo "), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
void PaintWindow(HWND hwnd, int iColor, int iFigure)
{
	static COLORREF iColors[] = { 
		RGB(0,0,0),RGB(0,0,255),
		RGB(0,255,0),RGB(0,255,255),
		RGB(255,0,0),RGB(255,0,255),
		RGB(255,255,0),RGB(255,255,255)
	};
	HBRUSH hBrush;
	RECT rect;
	HDC hdc;

	hBrush = CreateSolidBrush(iColors[iColor - IDC_RADIO1]);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (iFigure==IDC_RADIO9)
	{
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	else
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintBlock(HWND hwnd, int iColor, int iFigure)
{
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
	PaintWindow(hwnd, iColor, iFigure);

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hinst;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:
		hinst = ((LPCREATESTRUCT)lParam)->hInstance;
		
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_HELP_ABOUT:
			if(DialogBox(hinst, TEXT("About2"), hwnd, DialogProc))
				InvalidateRect(hwnd,NULL,TRUE);
		default:
			break;
		}
	case WM_PAINT:
		BeginPaint(hwnd,&ps);
		PaintWindow(hwnd, iCurrentColor,iCurrentFigure);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int iColor, iFigure;
	static  HWND  hCtrlBlock;
	switch (uMsg)
	{


	case WM_INITDIALOG:
		CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO8, iCurrentColor);
		CheckRadioButton(hwndDlg, IDC_RADIO9, IDC_RADIO10, iCurrentFigure);
		hCtrlBlock = GetDlgItem(hwndDlg, IDC_BLOCK);
		SetFocus(GetDlgItem(hwndDlg, IDC_BLOCK));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hwndDlg, TRUE);
			return TRUE;
		case IDCANCEL:
			EndDialog(hwndDlg, FALSE);
			return TRUE;
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
		case IDC_RADIO8:
			iColor = LOWORD(wParam);
			CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO8, iColor);
			PaintBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;
		case IDC_RADIO9:
		case IDC_RADIO10:
			iFigure = LOWORD(wParam);
			CheckRadioButton(hwndDlg, IDC_RADIO9, IDC_RADIO10, iFigure);
			PaintBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;
		}
		break;
	case WM_PAINT:
	PaintBlock(hCtrlBlock, iColor, iFigure);
		break;
	}
	return FALSE;
}