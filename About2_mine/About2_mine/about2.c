
#include"resource.h"
#include<windows.h>

int iCurrentColor = IDC_BLACK;
int iCurrentFigure = IDC_RECT;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void PaintWindow(HWND hwnd, int iColors, int iFigures);
void PaintBlock(HWND hwnd, int iColors, int iFigures);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int nCmdShow)
{
	static TCHAR szClassName[] = TEXT("About2");
	WNDCLASS wndclass;
	MSG msg;
	HWND hwnd;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hInstance, szClassName);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = szClassName;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("this program requires windows nt!"), szClassName, MB_ICONERROR | MB_OK);
		return 0;
	}
	hwnd = CreateWindow(szClassName, TEXT("About2 Demo program"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, NULL);

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
	static HINSTANCE  hinstance;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
		hinstance = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_HELP_ABOUT:
			DialogBox(hinstance, TEXT("About2"), hwnd, DialogProc);
			return 0;
		}
		break;
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		PaintWindow(hwnd, iCurrentColor, iCurrentFigure);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void PaintWindow(HWND hwnd, int iColors, int iFigures)
{
	static COLORREF crColor[8] = {
		RGB(0,0,0),RGB(0,0,255),
		RGB(0,255,0),RGB(0,255,255),
		RGB(255,0,0),RGB(255,0,255),
		RGB(255,255,0),RGB(255,255,255)
	};
	HDC hdc;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	hBrush = CreateSolidBrush(crColor[iColors - IDC_BLACK]);
	GetClientRect(hwnd, &rect);
	hdc = GetDC(hwnd);
	oldBrush = SelectObject(hdc, hBrush);
	if (iFigures == IDC_RECT)
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	else
	{
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}

	DeleteObject(SelectObject(hdc, oldBrush));
	ReleaseDC(hwnd, hdc);
}
void PaintBlock(HWND hwnd, int iColors, int iFigures)
{
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
	PaintWindow(hwnd, iColors, iFigures);
}
BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iColor, iFigure;
	static HWND hBlock;
	switch (message)
	{
	case WM_INITDIALOG:
		iColor = iCurrentColor;
		iFigure = iCurrentFigure;
		CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, iColor);
		CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, iFigure);
		hBlock = GetDlgItem(hDlg, IDC_BLOCK);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDC_BLACK:
		case IDC_BLUE:
		case IDC_CYAN:
		case IDC_GREEN:
		case IDC_MAGENTA:
		case IDC_RED:
		case IDC_YELLOW:
		case IDC_WHITE:
			iColor = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, LOWORD(wParam));
			PaintBlock(hBlock, iColor, iFigure);
			return TRUE;
		case IDC_RECT:
		case IDC_ELLIPSE:
			iFigure = LOWORD(wParam);
			CheckRadioButton(hBlock, IDC_RECT, IDC_ELLIPSE, LOWORD(wParam));
			PaintBlock(hBlock, iColor, iFigure);
			return TRUE;

		}
		break;
	case WM_PAINT:
		PaintBlock(hBlock, iColor, iFigure);
		break;
	}
	return FALSE;
}