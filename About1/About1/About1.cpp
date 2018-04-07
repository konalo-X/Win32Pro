#include<windows.h>
#include "resource.h"

int iCurrentColor=IDC_BLACK, iCurrentFigure=IDC_RECTANGLE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc2(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	TCHAR szAppName[] = TEXT("About1");
	HWND hwnd;
	MSG uMsg;
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
		MessageBox(NULL, TEXT("This Program requires windows NT!"), szAppName, MB_ICONEXCLAMATION);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("About 1 Dialog Box Demo program"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&uMsg,NULL,0,0))
	{
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}

	return uMsg.wParam;
}
void PaintWindow(HWND hwnddlg, int iColor, int iFigure)
{
 RECT rect;
	COLORREF colors[] = {RGB(0,0,0),RGB(0,0,255),
		RGB(0,255,0),RGB(0,255,255),
		RGB(255,0,0),RGB(255,0,255),
		RGB(255,255,0),RGB(255,255,255)
	};
	HDC hdc = GetDC(hwnddlg);
	GetClientRect(hwnddlg, &rect);
	HBRUSH hBrush = CreateSolidBrush(colors[iColor-IDC_BLACK  ]);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (iFigure==IDC_RECTANGLE)
	{
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	else
	{
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnddlg, hdc);
}

void PaintBlock(HWND hwndCtrl,int iColor,int iFigure)
{
	InvalidateRect(hwndCtrl, NULL, TRUE);
	UpdateWindow(hwndCtrl);
	PaintWindow(hwndCtrl, iColor, iFigure);
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
		case ID_HELP_HELP:
			DialogBox(hinst, TEXT("About1"), hwnd, DlgProc);
			return 0;
		case ID_ABOUT2:
			DialogBox(hinst, TEXT("About2"), hwnd, DlgProc2);
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		break;
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		
		EndPaint(hwnd, &ps);PaintWindow(hwnd, iCurrentColor, iCurrentFigure);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{	
			case	IDOK:
			case IDCANCEL:
				EndDialog(hwnd, 0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}

BOOL CALLBACK DlgProc2(HWND hwndDlg2, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	static HWND hwndCtrl;
	static int iColor, iFigure;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		iColor = iCurrentColor;
		iFigure=iCurrentFigure;
		CheckRadioButton(hwndDlg2, IDC_BLACK, IDC_WHITE, iColor);
		CheckRadioButton(hwndDlg2, IDC_RECTANGLE, IDC_ELLIPSE, iFigure);
		hwndCtrl = GetDlgItem(hwndDlg2, IDC_BLOCK);
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDOK:
			CheckRadioButton(hwndDlg2, IDC_BLACK, IDC_WHITE, iColor);
			CheckRadioButton(hwndDlg2, IDC_RECTANGLE, IDC_ELLIPSE, iFigure);
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hwndDlg2, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg2, 0);
			return FALSE;

		case IDC_BLACK:
		case IDC_BLUE:
		case IDC_GREEN:                       
		case IDC_CYAN:                       
		case IDC_RED :                        
		case IDC_MAGENTA :                  
		case IDC_YELLOW   :                   
		case IDC_WHITE  :                     
			iColor = LOWORD(wParam);
			PaintBlock(hwndCtrl, iColor, iFigure);
			//InvalidateRect(hwndCtrl, NULL, TRUE);
			return TRUE;
		case IDC_RECTANGLE :                 
		case IDC_ELLIPSE  :   
			iFigure = LOWORD(wParam);
			PaintBlock(hwndCtrl, iColor, iFigure);
			//InvalidateRect(hwndCtrl, NULL, TRUE);
			return TRUE;

		}
	
		break;
case WM_PAINT:
	PaintBlock(hwndCtrl, iColor, iFigure);
	break;
		
	}
	return FALSE;
}