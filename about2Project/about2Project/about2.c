#include<Windows.h>
#include"resource1.h"
LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int iCurrentColor = IDC_BLACK,
iCurrentFlag = ID_RECT;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("about2");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(hinstance, IDC_ARROW);
	wndclass.hIcon = LoadIcon(hinstance, szAppName);
	wndclass.hInstance = hinstance;
	wndclass.lpfnWndProc = WndPorc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = szAppName;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program reqiure windows nt!"), szAppName, MB_ICONERROR|MB_OK);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("About box demo program"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hinstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void Paintblock(HWND hctrl,int iColor,int iFigure)
{
	InvalidateRect(hctrl, NULL, TRUE);
	UpdateWindow(hctrl);
	RECT rect;
	HDC hdc = GetDC(hctrl);
	GetClientRect(hctrl, &rect);
	if (iFigure==ID_RECT)
	{
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}else
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	
	ReleaseDC(hctrl, hdc);

}

LRESULT CALLBACK WndPorc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hinstacne;
	switch (message)
	{
	case WM_CREATE:
		hinstacne = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_HELP:
			DialogBox(hinstacne, TEXT("about2"), hwnd, DialogProc);
			return 0;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
BOOL CALLBACK DialogProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static  int iColor, iFlag;
	static HWND hctrl; 
	switch (message)
	{
	case WM_INITDIALOG:
		iColor=iCurrentColor  ;
		iFlag=iCurrentFlag  ;
		CheckRadioButton(hdlg, IDC_BLACK, IDC_WHITE, iColor);
		CheckRadioButton(hdlg, ID_RECT, ID_Eclipse, iFlag);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BLACK:
		case IDC_BLUE:
		case	IDC_GREEN:
		case	IDC_CYAN:
		case	IDC_RED:
		case	IDC_MAGENTA:
		case	IDC_YELLOW:
		case	IDC_WHITE:
			iColor = LOWORD(wParam);
			CheckRadioButton(hdlg, IDC_BLACK, IDC_WHITE, iColor);
			hctrl=GetDlgItem(hdlg, IDC_Area);
			Paintblock(hctrl, iColor, iFlag);
			
			return 0;
		case	ID_RECT:
		case	ID_Eclipse:		
			iFlag = LOWORD(wParam);
			CheckRadioButton(hdlg, ID_RECT, ID_Eclipse, iFlag);
			hctrl = GetDlgItem(hdlg, IDC_Area);
			Paintblock(hctrl, iColor, iFlag);
			
			return 0;
		case IDOK:
			 iCurrentColor=iColor ;
			 iCurrentFlag=iFlag ;
			EndDialog(hdlg,1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hdlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}